#include "d3d11drv_pch.h"
#include "d3d11drv/d3d11renderer.h"
#include "d3d11drv/d3d11device.h"
#include "d3d11drv/d3d11shaderprogrammanager.h"
#include "d3d11drv/d3d11statemanager.h"

// filesystem
#include "core/file/filesystem.h"

// Base graphics classes
#include "graphics/view.h"

// Graphics objects
#include "graphics/image.h"
// #include "graphics/material.h"
// #include "graphics/materials/materialinstance.h"

// Graphics managers
// #include "graphics/ShaderProgramManager.h"
// #include "graphics/shaderconstantmanager.h"

#include "main/main.h"

#include <dxgidebug.h>

namespace engine
{

void createRenderer()
{
	g_renderer = mem_new<D3D11Renderer>();
}

void destroyRenderer()
{
	if (g_renderer)
	{
		mem_delete(g_renderer);
		g_renderer = nullptr;
	}
}

D3D11Renderer::D3D11Renderer() :
	m_renderTargetView(nullptr),
	m_depthStencilTexture(nullptr),
	m_depthStencilView(nullptr),
	m_depthStencilState(nullptr),
	m_rasterizerState(nullptr),
	m_swapChain(nullptr),
	m_takeScreenshot(false)
{
}

D3D11Renderer::~D3D11Renderer()
{
	m_takeScreenshot = false;
}

void D3D11Renderer::init()
{
	Core::msg("D3D11Renderer: Creating rendering device");

	// Initialize render device
	g_renderDevice = (IRenderDevice*)mem_new<D3D11Device>();
	((D3D11Device*)g_renderDevice)->create();

	// initalize swap chain
	createSwapChain();

	// initialize device state manager
	g_stateManager = mem_new<D3D11StateManager>();
	g_d3d11StateManager->init();

	createRasterizerState();

	// initialize base renderer after device creation
	Renderer::init();

	// Initialize shader manager with current api
	g_shaderManager = mem_new<D3D11ShaderProgramManager>();
	g_shaderManager->init("shaders/d3d11");
}

void D3D11Renderer::createSwapChain()
{
	D3D11Device* device = ((D3D11Device*)g_renderDevice);

	IDXGIDevice* dxgiDevice;
	device->getD3DDevice()->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter;
	HRESULT hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	HWND windowHandle = (HWND)appGetWindow();

	RECT rc = {};
	GetClientRect(windowHandle, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	memset(&swapChainDesc, 0, sizeof(swapChainDesc));
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = windowHandle;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	D3D11_CHECK(dxgiFactory->CreateSwapChain(device->getD3DDevice(), &swapChainDesc, &m_swapChain));

	ID3D11Texture2D* backBuffer;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	D3D11_CHECK(device->getD3DDevice()->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView));
	backBuffer->Release();
	backBuffer = nullptr;

	dxgiFactory->Release();
	dxgiAdapter->Release();
	dxgiDevice->Release();

	// Create depth stencil texture & view

	D3D11_TEXTURE2D_DESC depthStencilTextureDesc;
	memset(&depthStencilTextureDesc, 0, sizeof(depthStencilTextureDesc));
	depthStencilTextureDesc.Width = width;
	depthStencilTextureDesc.Height = height;
	depthStencilTextureDesc.MipLevels = 1;
	depthStencilTextureDesc.ArraySize = 1;
	depthStencilTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilTextureDesc.SampleDesc.Count = 1;
	depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	D3D11_CHECK(device->getD3DDevice()->CreateTexture2D(&depthStencilTextureDesc, NULL, &m_depthStencilTexture));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	memset(&depthStencilViewDesc, 0, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	D3D11_CHECK(device->getD3DDevice()->CreateDepthStencilView(m_depthStencilTexture, &depthStencilViewDesc, &m_depthStencilView));

	// Initialize target
	device->getDeviceContext()->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Create depth stencil state
	// https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-depth-stencil

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	memset(&depthStencilDesc, 0, sizeof(depthStencilDesc));

	// Depth test parameters
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	D3D11_CHECK(device->getD3DDevice()->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState));

	// Set depth stencil state
	device->getDeviceContext()->OMSetDepthStencilState(m_depthStencilState, 0);
}

void D3D11Renderer::createRasterizerState()
{
	RasterizerStateDesc rasterizerState;
	memset(&rasterizerState, 0, sizeof(rasterizerState));
	rasterizerState.m_cullMode = CullMode::Back;
	rasterizerState.m_frontCCW = true;
	rasterizerState.m_fillMode = FillMode::Solid;

	m_rasterizerState = g_stateManager->createRasterizerState(rasterizerState);
	m_rasterizerState = g_stateManager->createRasterizerState(rasterizerState);
}

void D3D11Renderer::shutdown()
{
	Renderer::shutdown();

	if (g_shaderManager)
	{
		mem_delete(g_shaderManager);
		g_shaderManager = nullptr;
	}

	g_d3d11StateManager->shutdown();

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = nullptr;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}

	if (m_depthStencilTexture)
	{
		m_depthStencilTexture->Release();
		m_depthStencilTexture = nullptr;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}

	mem_delete(g_stateManager);
	g_stateManager = nullptr;

	mem_delete(g_renderDevice);
	g_renderDevice = nullptr;
}

void D3D11Renderer::endFrame()
{
	//OPTICK_EVENT("D3D11Renderer::endFrame");

	if (m_takeScreenshot)
		takeScreenshotInternal();

	Renderer::endFrame();

	m_swapChain->Present(0, 0);
}

void D3D11Renderer::bindMaterialForMesh(MeshComponent* mesh, Material* material, MaterialInstance* materialInstance)
{
	// OPTICK_EVENT("D3D11Renderer::bindMaterialForMesh");

	Assert(mesh);
	Assert(material);
	Assert(materialInstance);

#if 0
	// bind material samplers
	material->bind();

	// Initialize shader
	IShaderProgram* shaderProgram = nullptr;
	
	if (mesh->isA<StaticMeshComponent>())
		shaderProgram = materialInstance->getStaticMeshShaderProgram();

	Assert2(shaderProgram, "Unknowed mesh component type!");

	// bind material instance shader and material uniforms
	g_shaderManager->setShaderProgram(shaderProgram);

	material->bindUniformsCustom(shaderProgram);

	// bind point lights
//	if (StaticMeshComponent* staticMesh = dynamicCast<StaticMeshComponent>(mesh))
//		ShaderConstantManager::getInstance()->setStaticMeshGlobalData(staticMesh, view, )

	//shaderProgram->setInteger("u_lightsCount", mesh->m_world->getWorldComponent<GraphicsWorld>()->getLightManager()->getLights().size());
	//ShaderConstantManager::getInstance()->setPointLightConstantBuffer();

#endif
}

void D3D11Renderer::renderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh)
{
	// OPTICK_EVENT("D3D11Renderer::renderMesh");

#if 0
	if (StaticMeshComponent* staticMesh = dynamicCast<StaticMeshComponent>(mesh))
		renderStaticMesh(graphicsWorld, view, staticMesh);
#endif
}

void D3D11Renderer::renderStaticMesh(GraphicsWorld* graphicsWorld, View* view, StaticMeshComponent* mesh)
{
	// OPTICK_EVENT("D3D11Renderer::renderStaticMesh");

#if 0

	for (const auto& submesh : mesh->getModel()->getSubmehes())
	{
		// create saved render ctx as previous model.
		RenderContext savedCtx = RenderContext::getContext();

		// create local copy of render context
		RenderContext localCtx = RenderContext::getContext();

		// and overwrite model matrix
		localCtx.model = savedCtx.model * submesh->getTransform();

		// set our local render ctx
		RenderContext::setContext(localCtx);

		g_renderDevice->setVertexBuffer(submesh->getVertexBuffer(), sizeof(Vertex), 0);
		g_renderDevice->setVertexFormat(&s_vfVertex);

		//g_renderDevice->setIndexBuffer(it->getIndexBuffer());

		//it->getMaterial()->bind();

		bindMaterialForMesh(mesh, submesh->getMaterial().get(), submesh->getMaterial()->getMaterialInstance());

		ShaderConstantManager::getInstance()->setStaticMeshGlobalData(mesh, view, localCtx, graphicsWorld);

		/*graphicsDevice->setIndexBuffer(it->getIndexBuffer());*/

		/*graphicsDevice->drawElements(PrimitiveMode::Triangles, it->getIndeciesCount());*/

		// install polygon fill mode based on which mode set now

		// if we showing polys
		if (m_meshPolysWireframe && m_currentViewMode != RendererViewMode::Wireframe)
		{
			// render mesh normaly
			//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);
			//glDrawArrays(GL_TRIANGLES, 0, it->getVerticesCount());

			// set polygon fill to lines
		//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			// hack view
			RenderContext hackHackHack = localCtx;
			hackHackHack.proj[2][3] -= 0.0001f;
			RenderContext::setContext(hackHackHack);

			// hack the view
			RendererViewMode savedViewMode = m_currentViewMode;
			m_currentViewMode = RendererViewMode::Wireframe;

			// bind material again
			bindMaterialForMesh(mesh, submesh->getMaterial().get(), submesh->getMaterial()->getMaterialInstance());

			// draw with lines
			g_renderDevice->draw(PM_TriangleList, 0, submesh->getVerticesCount());
			//glDrawArrays(GL_TRIANGLES, 0, it->getVerticesCount());
			//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);

			// reset view mode
			m_currentViewMode = savedViewMode;

			// reset mode
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
		//	if (getRenderMode() == RendererViewMode::Wireframe)
		//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			g_renderDevice->draw(PM_TriangleList, 0, submesh->getVerticesCount());
		//	glDrawArrays(GL_TRIANGLES, 0, it->getVerticesCount());
			//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);

			// reset
		//	if (getRenderMode() == RendererViewMode::Wireframe)
		//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// return what have been
		RenderContext::setContext(savedCtx);
	}

#endif
}

void D3D11Renderer::renderShadows(View* view)
{
}

void D3D11Renderer::takeScreenshot()
{
	m_takeScreenshot = true;
}

void D3D11Renderer::takeScreenshotInternal()
{
	int width = getView()->m_width;
	int height = getView()->m_height;
	int slicePitch = width * height;

	size_t bufferSize = 4 * width * height;
	uint8_t* screenBuffer = new uint8_t[bufferSize];

	D3D11Device* device = reinterpret_cast<D3D11Device*>(g_renderDevice);

	ID3D11Texture2D* screenTexture = nullptr;
	D3D11_CHECK(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&screenTexture));

	D3D11_TEXTURE2D_DESC textureDesc;
	screenTexture->GetDesc(&textureDesc);
	textureDesc.BindFlags = 0;
	textureDesc.MiscFlags &= D3D11_RESOURCE_MISC_TEXTURECUBE;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textureDesc.Usage = D3D11_USAGE_STAGING;

	ID3D11Texture2D* copyOfScreenTexture = nullptr;
	D3D11_CHECK(device->getD3DDevice()->CreateTexture2D(&textureDesc, NULL, &copyOfScreenTexture));

	device->getDeviceContext()->CopyResource(copyOfScreenTexture, screenTexture);

	screenTexture->Release();
	screenTexture = nullptr;

	// map resource
	D3D11_MAPPED_SUBRESOURCE mapped;
	D3D11_CHECK(device->getDeviceContext()->Map(copyOfScreenTexture, 0, D3D11_MAP_READ, 0, &mapped));

	uint64_t imageSize = uint64_t(mapped.RowPitch) * uint64_t(textureDesc.Height);
	memcpy(screenBuffer, mapped.pData, imageSize);

	device->getDeviceContext()->Unmap(copyOfScreenTexture, 0);

	copyOfScreenTexture->Release();
	copyOfScreenTexture = nullptr;

	Image image;
	image.createRaw((void*)screenBuffer, width, height, 4);
	image.setFlip(false);

	

	char buffer[256];
	for (int i = 0;; i++)
	{
		sprintf(buffer, "sshot_%i.png", i);

		FileHandle fh = g_fileSystem->open(buffer);
		if (!fh)
		{
			break;
		}
		else
		{
			g_fileSystem->close(fh);
		}
	}

	image.save(buffer);

	delete[] screenBuffer;

	m_takeScreenshot = false;
}

void D3D11Renderer::clearScreen()
{
	if (m_renderTargetView)
	{
		static float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		((D3D11Device*)g_renderDevice)->getDeviceContext()->ClearRenderTargetView(m_renderTargetView, color);
		((D3D11Device*)g_renderDevice)->getDeviceContext()->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
}

}