#ifndef GRAPHICS_GL_GLBUFFERIMPL_H
#define GRAPHICS_GL_GLBUFFERIMPL_H

namespace solunar
{

class GLBufferImpl : public IBufferBase
{
public:
	GLBufferImpl(const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc);
	~GLBufferImpl();

	void Create(const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc);
	void Destroy();

	uint32_t getHandle() { return m_handle; }

	void* Map(BufferMapping mapping) override;
	void  Unmap() override;

	void UpdateSubresource(void* data, size_t size) override;

	BufferDesc getBufferDesc() override { return m_bufferDesc; }

private:
	BufferDesc m_bufferDesc;
	uint32_t m_handle;
};

}

#endif // !GRAPHICS_GL_GLBUFFERIMPL_H
