#include "d3d11drv_pch.h"
#include "d3d11drv/d3d11viewsurface.h"
#include "d3d11drv/d3d11device.h"

namespace engine
{

D3D11ViewSurface::D3D11ViewSurface(void* handle) :
	m_renderTargetView(nullptr),
	m_depthStencilTexture(nullptr),
	m_depthStencilView(nullptr),
	m_swapChain(nullptr),
	m_windowHandle(handle)
{
	Assert(m_windowHandle);

	IDXGIDevice* dxgiDevice;
	g_d3d11Device->getDevice()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter;
	HRESULT hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	HWND windowHandle = (HWND)m_windowHandle;

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
	D3D11_CHECK(dxgiFactory->CreateSwapChain(g_d3d11Device->getDevice(), &swapChainDesc, &m_swapChain));

	ID3D11Texture2D* backBuffer;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	D3D11_CHECK(g_d3d11Device->getDevice()->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView));
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

	D3D11_CHECK(g_d3d11Device->getDevice()->CreateTexture2D(&depthStencilTextureDesc, NULL, &m_depthStencilTexture));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	memset(&depthStencilViewDesc, 0, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	D3D11_CHECK(g_d3d11Device->getDevice()->CreateDepthStencilView(m_depthStencilTexture, &depthStencilViewDesc, &m_depthStencilView));
}

D3D11ViewSurface::~D3D11ViewSurface()
{
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
}

void D3D11ViewSurface::display(int frameSyncNum)
{
	// Initialize target
	g_d3d11Device->getDeviceContext()->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// clear target
	static float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	g_d3d11Device->getDeviceContext()->ClearRenderTargetView(m_renderTargetView, color);
	g_d3d11Device->getDeviceContext()->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Present
	m_swapChain->Present(frameSyncNum, 0);
}

}

using namespace engine;

IViewSurface* createPlatfromViewSurface(void* handle)
{
	IViewSurface* viewSurface = mem_new<D3D11ViewSurface>(handle);
	return viewSurface;
}
