#include "d3d11drv_pch.h"
#include "d3d11drv/d3d11bufferimpl.h"

namespace solunar
{

D3D11BufferImpl::D3D11BufferImpl(D3D11Device* device, const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc) :
	m_buffer(nullptr),
	m_device(nullptr),
	m_bufferDesc(bufferDesc)
{
	Create(device, bufferDesc, subresourceDesc);
}

D3D11BufferImpl::~D3D11BufferImpl()
{
	Destroy();
}

void D3D11BufferImpl::Create(D3D11Device* device, const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc)
{
	Assert2(device, "Failed to create buffer without initialized device.");
	m_device = device;
	
	D3D11_BUFFER_DESC d3d11BufferDesc;
	memset(&d3d11BufferDesc, 0, sizeof(d3d11BufferDesc));
	d3d11BufferDesc.Usage = GetD3D11Usage(bufferDesc);
	d3d11BufferDesc.ByteWidth = bufferDesc.m_bufferMemorySize;
	d3d11BufferDesc.BindFlags = GetD3D11BindFlags(bufferDesc);
	d3d11BufferDesc.CPUAccessFlags = (bufferDesc.m_bufferAccess == BufferAccess::Dynamic || bufferDesc.m_bufferAccess == BufferAccess::Stream) ? D3D11_CPU_ACCESS_WRITE : 0;
	d3d11BufferDesc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA subresourceData;
	memset(&subresourceData, 0, sizeof(subresourceData));
	subresourceData.pSysMem = subresourceDesc.m_memory;
	subresourceData.SysMemPitch = subresourceDesc.m_memoryPitch;
	subresourceData.SysMemSlicePitch = subresourceDesc.m_memorySlicePitch;
	
	D3D11_CHECK(device->getDevice()->CreateBuffer(&d3d11BufferDesc, subresourceDesc.m_memory ? &subresourceData : NULL, &m_buffer));
}

void D3D11BufferImpl::Destroy()
{
	if (m_buffer)
	{
		m_buffer->Release();
		m_buffer = nullptr;
	}

	m_device = nullptr;
}

void* D3D11BufferImpl::Map(BufferMapping mapping)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	memset(&mappedSubresource, 0, sizeof(mappedSubresource));
	
	D3D11_CHECK(m_device->getDeviceContext()->Map(m_buffer, 0, GetD3D11Map(mapping), 0, &mappedSubresource));
	
	return mappedSubresource.pData;
}

void D3D11BufferImpl::Unmap()
{
	m_device->getDeviceContext()->Unmap(m_buffer, 0);
}

void D3D11BufferImpl::UpdateSubresource(void* data, size_t size)
{
	m_device->getDeviceContext()->UpdateSubresource(m_buffer, 0, NULL, data, 0, 0);
}

}