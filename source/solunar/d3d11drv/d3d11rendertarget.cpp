#include "d3d11drv_pch.h"
#include "d3d11drv/d3d11rendertarget.h"
#include "d3d11drv/d3d11texture2d.h"

namespace engine
{

extern DXGI_FORMAT getDxgiFormat(ImageFormat format);

D3D11RenderTarget::D3D11RenderTarget(ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView)
{
	memset(m_renderTargetViews.data(), 0, m_renderTargetViews.size() * sizeof(ID3D11RenderTargetView));
	memset(m_shaderResourceViews.data(), 0, m_shaderResourceViews.size() * sizeof(ID3D11ShaderResourceView));

	// clear the render target desc
	memset(&m_renderTargetDesc, 0, sizeof(m_renderTargetDesc));

	if (renderTargetView)
	{
		// add reference for properly releasing in desctuctor
		renderTargetView->AddRef();
		
		// assign
		m_renderTargetViews[m_renderTargetDesc.m_textures2DCount++] = renderTargetView;
	}

	if (depthStencilView)
	{
		// add reference for properly releasing in desctuctor
		depthStencilView->AddRef();

		// assign
		m_depthStencilView = depthStencilView;
	}
}

D3D11RenderTarget::D3D11RenderTarget(D3D11Device* device, const RenderTargetCreationDesc& renderTargetDesc) :
	m_renderTargetDesc(renderTargetDesc),
	m_depthStencilView(nullptr)
{
	memset(m_renderTargetViews.data(), 0, m_renderTargetViews.size() * sizeof(ID3D11RenderTargetView));
	memset(m_shaderResourceViews.data(), 0, m_shaderResourceViews.size() * sizeof(ID3D11ShaderResourceView));

	create(device, renderTargetDesc);
}

D3D11RenderTarget::~D3D11RenderTarget()
{
	release();
}

void D3D11RenderTarget::create(D3D11Device* device, const RenderTargetCreationDesc& renderTargetDesc)
{
	for (int i = 0; i < renderTargetDesc.m_textures2DCount; i++)
	{
		D3D11Texture2D* texture2D = (D3D11Texture2D*)renderTargetDesc.m_textures2D[i];
		Assert(texture2D);

		TextureDesc textureDesc = texture2D->getTextureDesc();

		ID3D11Texture2D* d3dTexture = texture2D->getTexture();
		Assert(d3dTexture);

		// Create render target view

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		memset(&renderTargetViewDesc, 0, sizeof(renderTargetViewDesc));
		renderTargetViewDesc.Format = getDxgiFormat(textureDesc.m_format);
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		ID3D11RenderTargetView* renderTargetView = nullptr;
		D3D11_CHECK(device->getDevice()->CreateRenderTargetView(d3dTexture, &renderTargetViewDesc, &renderTargetView));

		m_renderTargetViews[i] = renderTargetView;

		// Create shader resource view

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		memset(&shaderResourceViewDesc, 0, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = getDxgiFormat(textureDesc.m_format);
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		
		ID3D11ShaderResourceView* shaderResourceView = nullptr;
		D3D11_CHECK(device->getDevice()->CreateShaderResourceView(d3dTexture, &shaderResourceViewDesc, &shaderResourceView));
		
		m_shaderResourceViews[i] = shaderResourceView;
	}

	// Create depth render target
	
	if (renderTargetDesc.m_depthTexture2D)
	{
		// get native texture
		D3D11Texture2D* texture2D = (D3D11Texture2D*)renderTargetDesc.m_depthTexture2D;
		Assert(texture2D);

		// create depth stencil view

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		memset(&depthStencilViewDesc, 0, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// #TODO: !!!! FIX !!!!!
		if (texture2D->getTextureDesc().m_format == ImageFormat::DEPTH32F)
			depthStencilViewDesc.Format = getDxgiFormat(texture2D->getTextureDesc().m_format);
		else
			depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		// #TODO: !!!! FIX !!!!!
	/*	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		memset(&depthStencilViewDesc, 0, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;*/

		D3D11_CHECK(device->getDevice()->CreateDepthStencilView(texture2D->getTexture(), &depthStencilViewDesc, &m_depthStencilView));
	}
}

void D3D11RenderTarget::release()
{
	//for (ID3D11ShaderResourceView** it = m_shaderResourceViews.begin(); it != m_shaderResourceViews.end(); ++it)
	//{
	//	ID3D11ShaderResourceView* handle = (*it);
	//	if (handle)
	//	{
	//		handle->Release();
	//		handle = nullptr;
	//	}
	//}

	//for (ID3D11RenderTargetView** it = m_renderTargetViews.begin(); it != m_renderTargetViews.end(); ++it)
	//{
	//	ID3D11RenderTargetView* handle = (*it);
	//	if (handle)
	//	{
	//		handle->Release();
	//		handle = nullptr;
	//	}
	//}

	for (auto& it : m_shaderResourceViews)
	{
		if (it)
			it->Release();
	}

	for (auto& it : m_renderTargetViews)
	{
		if (it)
			it->Release();
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}
}

void D3D11RenderTarget::bind(D3D11Device* device)
{
	ID3D11DeviceContext* deviceContext = device->getDeviceContext();

	for (int i = 0; i < m_renderTargetDesc.m_textures2DCount; i++)
	{
		// #TODO: Hack with depth stencil installation
		deviceContext->OMSetRenderTargets(1, &m_renderTargetViews[i], (i == 0) ? m_depthStencilView : nullptr);
		deviceContext->PSGetShaderResources(i, 1, &m_shaderResourceViews[i]);
	}

	// #TODO: !!!
	if (m_renderTargetDesc.m_depthTexture2D && m_renderTargetDesc.m_textures2DCount==0)
	{
		deviceContext->OMSetRenderTargets(0, nullptr, m_depthStencilView);
	}
}

}
