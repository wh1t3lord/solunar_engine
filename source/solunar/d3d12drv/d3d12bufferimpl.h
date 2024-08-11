#ifndef GRAPHICS_D3D12_D3D12BUFFERIMPL_H
#define GRAPHICS_D3D12_D3D12BUFFERIMPL_H

#include "d3d12drv/d3d12device.h"

namespace solunar
{

class D3D12BufferImpl : public IBufferBase
{
public:
	D3D12BufferImpl(D3D12Device* device, const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc);
	~D3D12BufferImpl();

	void Create(D3D12Device* device, const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc);
	void Destroy();

	void* Map(BufferMapping mapping) override;
	void Unmap() override;

	void updateSubresource(void * data, size_t size) override;

	BufferDesc getBufferDesc() override { return m_bufferDesc; }

	ID3D12Resource* getBuffer() { return m_buffer; }

	const D3D12_VERTEX_BUFFER_VIEW& getVertexBufferView() { return m_vertexBufferView; }
	const D3D12_INDEX_BUFFER_VIEW& getIndexBufferView() { return m_indexBufferView; }

	// #TODO :	HACK HACK HACK
	//			Ugly very ugly
	void setIndexFormat(bool is16BitIndices);

private:
	BufferDesc m_bufferDesc;

	ID3D12Resource* m_buffer;

	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

	D3D12Device* m_device;
};

}

#endif // !GRAPHICS_D3D12_D3D12BUFFERIMPL_H
