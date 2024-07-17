#ifndef GRAPHICS_GL_GLBUFFERIMPL_H
#define GRAPHICS_GL_GLBUFFERIMPL_H

namespace engine
{

class GLBufferImpl : public IBufferBase
{
public:
	GLBufferImpl(const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc);
	~GLBufferImpl();

	void create(const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc);
	void destroy();

	uint32_t getHandle() { return m_handle; }

	void* map(BufferMapping mapping) override;
	void  unmap() override;

	void updateSubresource(void* data, size_t size) override;

	BufferDesc getBufferDesc() override { return m_bufferDesc; }

private:
	BufferDesc m_bufferDesc;
	uint32_t m_handle;
};

}

#endif // !GRAPHICS_GL_GLBUFFERIMPL_H
