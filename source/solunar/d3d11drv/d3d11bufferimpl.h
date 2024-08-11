#ifndef GRAPHICS_D3D11_D3D11BUFFERIMPL_H
#define GRAPHICS_D3D11_D3D11BUFFERIMPL_H

#include "d3d11drv/d3d11device.h"

namespace solunar
{

class D3D11BufferImpl : public IBufferBase
{
public:
	D3D11BufferImpl(D3D11Device* device, const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc);
	~D3D11BufferImpl();

	void Create(D3D11Device* device, const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc);
	void Destroy();

	void* Map(BufferMapping mapping) override;
	void Unmap() override;

	void updateSubresource(void * data, size_t size) override;

	BufferDesc getBufferDesc() override { return m_bufferDesc; }

	ID3D11Buffer* getBuffer() { return m_buffer; }

private:
	BufferDesc m_bufferDesc;

	ID3D11Buffer* m_buffer;
	D3D11Device* m_device;
};

}

#endif // !GRAPHICS_D3D11_D3D11BUFFERIMPL_H
