#ifndef GRAPHICS_D3D12_D3D12BUFFERIMPL_H
#define GRAPHICS_D3D12_D3D12BUFFERIMPL_H

#include "d3d12drv/d3d12device.h"

namespace engine
{

class D3D12BufferImpl : public IBufferBase
{
public:
	D3D12BufferImpl(D3D12Device* device, const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc);
	~D3D12BufferImpl();

	void create(D3D12Device* device, const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc);
	void destroy();

	void* map(BufferMapping mapping) override;
	void unmap() override;

	void updateSubresource(void * data, size_t size) override;

	BufferDesc getBufferDesc() override { return m_bufferDesc; }

	ID3D12Resource* getBuffer() { return m_buffer; }
	const D3D12_VERTEX_BUFFER_VIEW& getBufferView() { return m_bufferView; }

private:
	BufferDesc m_bufferDesc;

	ID3D12Resource* m_buffer;
	D3D12_VERTEX_BUFFER_VIEW m_bufferView;

	D3D12Device* m_device;
};

}

#endif // !GRAPHICS_D3D12_D3D12BUFFERIMPL_H
