#include "d3d12drv_pch.h"
#include "d3d12drv/d3d12device.h"
#include "d3d12drv/d3d12bufferimpl.h"
//#include "d3d12drv/d3d12rendertarget.h"
//#include "d3d12drv/d3d12texture2d.h"
//#include "d3d12drv/d3d12samplerstate.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "dxguid.lib")

namespace engine
{

D3D12_PRIMITIVE_TOPOLOGY getD3D12PrimitiveTopology(PrimitiveMode primitiveMode)
{
	switch (primitiveMode)
	{
	case PM_LineList:
		return D3D_PRIMITIVE_TOPOLOGY_LINELIST;

	case PM_LineStrip:
		return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;

	case PM_TriangleList:
		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	case PM_TriangleStrip:
		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	
	default:
		break;
	}

	Assert2(0, "Unknowed or unsupported primitive mode");
	return (D3D12_PRIMITIVE_TOPOLOGY)0;
}

D3D12Device::D3D12Device() :
	m_device(nullptr),
	m_commandList(nullptr)
{
	memset(&m_viewport, 0, sizeof(m_viewport));
}

D3D12Device::~D3D12Device()
{
	destroy();
}

void D3D12Device::create()
{
	// Find adapter for d3d12 device

	UINT createFactoryFlags = 0;
#if defined(_DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	IDXGIFactory4* dxgiFactory = nullptr;
	D3D12_CHECK(CreateDXGIFactory2(createFactoryFlags, __uuidof(IDXGIFactory4), reinterpret_cast<void**>(&dxgiFactory)));

	// find based of total video memory

	SIZE_T maxDedicatedVideoMemory = 0;
	IDXGIAdapter1* dxgiAdapter = nullptr;
	IDXGIAdapter4* dxgiAdapter4 = nullptr;

	for (UINT i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
		D3D12_CHECK(dxgiAdapter->GetDesc1(&dxgiAdapterDesc1));

		if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
			SUCCEEDED(D3D12CreateDevice(dxgiAdapter,
				D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
			dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory)
		{
			maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
			D3D12_CHECK(dxgiAdapter->QueryInterface(&dxgiAdapter4));
		}
	}

	// Create D3D12 device
	D3D12_CHECK(D3D12CreateDevice(dxgiAdapter4, D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), reinterpret_cast<void**>(&m_device)));

	DXGI_ADAPTER_DESC adapterDesc;
	dxgiAdapter4->GetDesc(&adapterDesc);

	Core::msg("D3D12RenderDevice: GPU: %S, Video Memory: %zu MB",
		adapterDesc.Description,
		adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Release adapters
	dxgiAdapter4->Release(); dxgiAdapter4 = nullptr;
	//dxgiAdapter->Release(); dxgiAdapter = nullptr;

}

void D3D12Device::destroy()
{
	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}
}

IRenderTarget* D3D12Device::createRenderTarget(const RenderTargetCreationDesc& renderTargetDesc)
{
	//return mem_new<D3D12RenderTarget>(this, renderTargetDesc);
	return nullptr;
}

IBufferBase* D3D12Device::createBuffer(const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc)
{
	return mem_new<D3D12BufferImpl>(this, bufferDesc, subresourceDesc);
}

ITexture2D* D3D12Device::createTexture2D(const TextureDesc& textureDesc, const SubresourceDesc& subresourceDesc)
{
//	return mem_new<D3D12Texture2D>(this, textureDesc, subresourceDesc);
	return nullptr;
}

ISamplerState* D3D12Device::createSamplerState(const SamplerDesc& samplerDesc)
{
//	return mem_new<D3D12SamplerState>(this, samplerDesc);
	return nullptr;
}

void D3D12Device::setRenderTarget(IRenderTarget* rt)
{
#if 0
	if (rt)
	{
		D3D12RenderTarget* renderTarget = (D3D12RenderTarget*)rt;
		renderTarget->bind(this);
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			// #TODO: Hack with depth stencil installation
			m_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
			m_deviceContext->PSGetShaderResources(i, 0, nullptr);
		}
	}
#endif

	//if (rt)
	//{
	//	D3D12RenderTarget* renderTarget = (D3D12RenderTarget*)rt;
	//	//ID3D12RenderTargetView* pD3DRenderTargetView = renderTarget->getRenderTargetView();

	//	//m_deviceContext->OMSetRenderTargets(1, &pD3DRenderTargetView, NULL);
	//}
	//else
	//{
	//	m_deviceContext->OMSetRenderTargets(0, NULL, NULL);
	//}
}

void D3D12Device::setConstantBuffer(IBufferBase* cb)
{
	Assert2(0, "D3D12Device::setConstantBuffer is obsolote. Please use setConstantBufferIndex");
	setConstantBufferIndex(0, cb);
}

void D3D12Device::setConstantBufferIndex(int slot, IBufferBase* cb)
{
	//D3D12BufferImpl* bufferImpl = (D3D12BufferImpl*)cb;
	//ID3D12Buffer* pD3DBuffer = bufferImpl->getBuffer();
	//
	//m_deviceContext->VSSetConstantBuffers(slot, 1, &pD3DBuffer);
	//m_deviceContext->PSSetConstantBuffers(slot, 1, &pD3DBuffer);
}

void D3D12Device::setVertexBuffer(IBufferBase* buffer, uint32_t stride, uint32_t offset)
{
	//D3D12BufferImpl* bufferImpl = (D3D12BufferImpl*)buffer;
	//ID3D12Buffer* pD3DBuffer = bufferImpl->getBuffer();
	//m_deviceContext->IASetVertexBuffers(0, 1, &pD3DBuffer, &stride, &offset);
}

void D3D12Device::setIndexBuffer(IBufferBase* buffer, bool use16bitsIndices)
{
	//D3D12BufferImpl* bufferImpl = (D3D12BufferImpl*)buffer;
	//ID3D12Buffer* pD3DBuffer = bufferImpl->getBuffer();
	//
	//m_deviceContext->IASetIndexBuffer(pD3DBuffer, use16bitsIndices ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
}

void D3D12Device::setTexture2D(int slot, ITexture2D* texture)
{
#if 0
	D3D12Texture2D* nativeTexture = (D3D12Texture2D*)texture;
	if (nativeTexture)
	{
		ID3D12ShaderResourceView* textureSRV = nativeTexture->getTextureSRV();
		if (textureSRV)
		{
			m_deviceContext->PSSetShaderResources(slot, 1, &textureSRV);
		}
	}
	else
	{
		m_deviceContext->PSSetShaderResources(slot, 0, nullptr);
	}
#endif
}

void D3D12Device::setSampler(int slot, ISamplerState* sampler)
{
#if 0
	D3D12SamplerState* samplerImpl = (D3D12SamplerState*)sampler;
	if (samplerImpl)
	{
		ID3D12SamplerState* pD3DSamplerState = samplerImpl->getSamplerState();
		m_deviceContext->PSSetSamplers(slot, 1, &pD3DSamplerState);
	}
	else
	{
		m_deviceContext->PSSetSamplers(slot, 0, nullptr);
	}
#endif
}

void D3D12Device::setViewport(Viewport* viewport)
{
	Assert(viewport);

	// fill d3d12 viewport
	D3D12_VIEWPORT vp;
	memset(&vp, 0, sizeof(vp));
	vp.TopLeftX = viewport->m_x;
	vp.TopLeftY= viewport->m_y;
	vp.Width = viewport->m_width;
	vp.Height = viewport->m_height;
	vp.MinDepth = 0.1f;
	vp.MaxDepth = 1.0000f;
//	m_deviceContext->RSSetViewports(1, &vp);

	// copy viewport to device
	m_viewport = *viewport;
}

Viewport D3D12Device::getViewport()
{
	return m_viewport;
}

void D3D12Device::setScissors(float x, float y, float w, float h)
{
	D3D12_RECT rect = { static_cast<LONG>(x), static_cast<LONG>(y), static_cast<LONG>(w), static_cast<LONG>(h) };
	//m_deviceContext->RSSetScissorRects(1, &rect);
}

void D3D12Device::draw(PrimitiveMode primitiveMode, size_t verticesStart, size_t verticesCount)
{
	//m_deviceContext->IASetPrimitiveTopology(getD3D12PrimitiveTopology(primitiveMode));
	//m_deviceContext->Draw(verticesCount, verticesStart);
}

void D3D12Device::drawIndexed(PrimitiveMode primitiveMode, size_t indexStart, size_t indexCount, int baseVertexLocation)
{
	//m_deviceContext->IASetPrimitiveTopology(getD3D12PrimitiveTopology(primitiveMode));
	//m_deviceContext->DrawIndexed(indexCount, indexStart, baseVertexLocation);
}

}