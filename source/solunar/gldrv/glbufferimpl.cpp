#include "gldrv_pch.h"
#include "gldrv/glbufferimpl.h"

namespace engine
{
	
extern GLenum getBufferAccess(BufferAccess access);
extern GLenum getBufferMapping(BufferMapping mapping);

GLBufferImpl::GLBufferImpl(const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc)
{
	create(bufferDesc, subresourceDesc);
}

GLBufferImpl::~GLBufferImpl()
{
	destroy();
}

void GLBufferImpl::create(const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc)
{
	Assert(bufferDesc.m_bufferMemorySize != 0);

	m_bufferDesc = bufferDesc;

	glGenBuffers(1, &m_handle);
	glNamedBufferDataEXT(m_handle, bufferDesc.m_bufferMemorySize, 
		subresourceDesc.m_memory, getBufferAccess(bufferDesc.m_bufferAccess));
}

void GLBufferImpl::destroy()
{
	glDeleteBuffers(1, &m_handle);
}

void* GLBufferImpl::map(BufferMapping mapping)
{
	void* ret = glMapNamedBufferEXT(m_handle, getBufferMapping(mapping));
	return ret;
}

void GLBufferImpl::unmap()
{
	glUnmapNamedBufferEXT(m_handle);
}

void GLBufferImpl::updateSubresource(void* data, size_t size)
{
	glNamedBufferSubDataEXT(m_handle, 0, size, data);
}


}