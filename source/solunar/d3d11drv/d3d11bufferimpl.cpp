#include "d3d11drv_pch.h"
#include "d3d11drv/d3d11bufferimpl.h"

namespace engine
{

UINT getD3D11BindFlags(const BufferDesc& bufferDesc)
{
	switch (bufferDesc.m_bufferType)
	{
		case BufferType::VertexBuffer:
			return D3D11_BIND_VERTEX_BUFFER;
			
		case BufferType::IndexBuffer:
			return D3D11_BIND_INDEX_BUFFER;
			
		case BufferType::ConstantBuffer:
			return D3D11_BIND_CONSTANT_BUFFER;
			
		default:
			break;
	}
	
	Assert2(0, "Unknowed buffer type");
	return 0;
}

D3D11_MAP getD3D11Map(BufferMapping mapping)
{
	switch (mapping)
	{
		case BufferMapping::ReadOnly:
			return D3D11_MAP_READ;
		
		case BufferMapping::WriteOnly:
			return D3D11_MAP_WRITE_DISCARD;
			
		case BufferMapping::ReadAndWrite:
			return D3D11_MAP_READ_WRITE;
			
		default:
			break;
	}
	
	Assert2(0, "Unknowed buffer mapping type");
	return (D3D11_MAP)0;
}

D3D11_USAGE getD3D11Usage(const BufferDesc& desc)
{
	switch (desc.m_bufferAccess)
	{
	case BufferAccess::Static:
		return D3D11_USAGE_DEFAULT;

	case BufferAccess::Dynamic:
	case BufferAccess::Stream:
		return D3D11_USAGE_DYNAMIC;

	default:
		break;
	}

	Assert2(0, "Unknowed buffer access type");
	return (D3D11_USAGE)0;
}

D3D11BufferImpl::D3D11BufferImpl(D3D11Device* device, const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc) :
	m_buffer(nullptr),
	m_device(nullptr)
{
	create(device, bufferDesc, subresourceDesc);
}

D3D11BufferImpl::~D3D11BufferImpl()
{
	destroy();
}

void D3D11BufferImpl::create(D3D11Device* device, const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc)
{
	Assert2(device, "Failed to create buffer without initialized device.");
	m_device = device;
	
	D3D11_BUFFER_DESC d3d11BufferDesc;
	memset(&d3d11BufferDesc, 0, sizeof(d3d11BufferDesc));
	d3d11BufferDesc.Usage = getD3D11Usage(bufferDesc);
	d3d11BufferDesc.ByteWidth = bufferDesc.m_bufferMemorySize;
	d3d11BufferDesc.BindFlags = getD3D11BindFlags(bufferDesc);
	d3d11BufferDesc.CPUAccessFlags = (bufferDesc.m_bufferAccess == BufferAccess::Dynamic || bufferDesc.m_bufferAccess == BufferAccess::Stream) ? D3D11_CPU_ACCESS_WRITE : 0;
	d3d11BufferDesc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA subresourceData;
	memset(&subresourceData, 0, sizeof(subresourceData));
	subresourceData.pSysMem = subresourceDesc.m_memory;
	subresourceData.SysMemPitch = subresourceDesc.m_memoryPitch;
	subresourceData.SysMemSlicePitch = subresourceDesc.m_memorySlicePitch;
	
	D3D11_CHECK(device->getD3DDevice()->CreateBuffer(&d3d11BufferDesc, subresourceDesc.m_memory ? &subresourceData : NULL, &m_buffer));
}

void D3D11BufferImpl::destroy()
{
	if (m_buffer)
	{
		m_buffer->Release();
		m_buffer = nullptr;
	}

	m_device = nullptr;
}

void* D3D11BufferImpl::map(BufferMapping mapping)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	memset(&mappedSubresource, 0, sizeof(mappedSubresource));
	
	D3D11_CHECK(m_device->getDeviceContext()->Map(m_buffer, 0, getD3D11Map(mapping), 0, &mappedSubresource));
	
	return mappedSubresource.pData;
}

void D3D11BufferImpl::unmap()
{
	m_device->getDeviceContext()->Unmap(m_buffer, 0);
}

void D3D11BufferImpl::updateSubresource(void* data, size_t size)
{
	m_device->getDeviceContext()->UpdateSubresource(m_buffer, 0, NULL, data, 0, 0);
}

}