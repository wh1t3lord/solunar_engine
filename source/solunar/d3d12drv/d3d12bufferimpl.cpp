#include "d3d12drv_pch.h"
#include "d3d12drv/d3d12bufferimpl.h"

namespace solunar
{

#if 0
UINT getD3D12BindFlags(const BufferDesc& bufferDesc)
{
	switch (bufferDesc.m_bufferType)
	{
		case BufferType::VertexBuffer:
			return D3D12_BIND_VERTEX_BUFFER;
			
		case BufferType::IndexBuffer:
			return D3D12_BIND_INDEX_BUFFER;
			
		case BufferType::ConstantBuffer:
			return D3D12_BIND_CONSTANT_BUFFER;
			
		default:
			break;
	}
	
	Assert2(0, "Unknowed buffer type");
	return 0;
}

D3D12_MAP getD3D12Map(BufferMapping mapping)
{
	switch (mapping)
	{
		case BufferMapping::ReadOnly:
			return D3D12_MAP_READ;
		
		case BufferMapping::WriteOnly:
			return D3D12_MAP_WRITE_DISCARD;
			
		case BufferMapping::ReadAndWrite:
			return D3D12_MAP_READ_WRITE;
			
		default:
			break;
	}
	
	Assert2(0, "Unknowed buffer mapping type");
	return (D3D12_MAP)0;
}

D3D12_USAGE getD3D12Usage(const BufferDesc& desc)
{
	switch (desc.m_bufferAccess)
	{
	case BufferAccess::Static:
		return D3D12_USAGE_DEFAULT;

	case BufferAccess::Dynamic:
	case BufferAccess::Stream:
		return D3D12_USAGE_DYNAMIC;

	default:
		break;
	}

	Assert2(0, "Unknowed buffer access type");
	return (D3D12_USAGE)0;
}
#endif

D3D12BufferImpl::D3D12BufferImpl(D3D12Device* device, const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc) :
	m_buffer(nullptr),
	m_device(nullptr),
	m_bufferDesc(bufferDesc)
{
	memset(&m_vertexBufferView, 0, sizeof(m_vertexBufferView));
	memset(&m_indexBufferView, 0, sizeof(m_indexBufferView));

	Create(device, bufferDesc, subresourceDesc);
}

D3D12BufferImpl::~D3D12BufferImpl()
{
	Destroy();
}

void D3D12BufferImpl::Create(D3D12Device* device, const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc)
{
	Assert2(device, "Failed to create buffer without initialized device.");
	m_device = device;
	
	D3D12_HEAP_PROPERTIES heapProperties;
	memset(&heapProperties, 0, sizeof(heapProperties));
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resourceDesc;
	memset(&resourceDesc, 0, sizeof(resourceDesc));
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = bufferDesc.m_bufferMemorySize;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE; //flags;

	D3D12_CHECK(device->getDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_buffer)));

	// Copy the triangle data to the vertex buffer.
	UINT8* pDataBegin = nullptr;
	D3D12_RANGE readRange = { 0, 0 };        // We do not intend to read from this resource on the CPU.
	D3D12_CHECK(m_buffer->Map(0, &readRange, reinterpret_cast<void**>(&pDataBegin)));
	memcpy(pDataBegin, subresourceDesc.m_memory, bufferDesc.m_bufferMemorySize);
	m_buffer->Unmap(0, nullptr);

	if (bufferDesc.m_bufferType == BufferType::VertexBuffer)
	{
		m_vertexBufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = subresourceDesc.m_memoryPitch;
		m_vertexBufferView.SizeInBytes = bufferDesc.m_bufferMemorySize;
	}
	else if (bufferDesc.m_bufferType == BufferType::IndexBuffer)
	{
		m_indexBufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();
		m_indexBufferView.SizeInBytes = bufferDesc.m_bufferMemorySize;
	}
}

void D3D12BufferImpl::Destroy()
{
	memset(&m_vertexBufferView, 0, sizeof(m_vertexBufferView));

	if (m_buffer)
	{
		m_buffer->Release();
		m_buffer = nullptr;
	}

	m_device = nullptr;
}

void* D3D12BufferImpl::Map(BufferMapping mapping)
{
	UINT8* data = nullptr;
	D3D12_RANGE readRange = { 0, 0 };        // We do not intend to read from this resource on the CPU.
	D3D12_CHECK(m_buffer->Map(0, &readRange, reinterpret_cast<void**>(&data)));
	return data;
}

void D3D12BufferImpl::Unmap()
{
	m_buffer->Unmap(0, nullptr);
}

void D3D12BufferImpl::UpdateSubresource(void* data, size_t size)
{
	Assert2(0, "Implement please");
//	m_device->GetDeviceContext()->UpdateSubresource(m_buffer, 0, NULL, data, 0, 0);
}

void D3D12BufferImpl::setIndexFormat(bool is16BitIndices)
{
	m_indexBufferView.Format = is16BitIndices ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
}

}