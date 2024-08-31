#include "gldrv_pch.h"
#include "gldrv/glbufferimpl.h"

namespace solunar
{
	
extern GLenum GetBufferAccess(BufferAccess access);
extern GLenum GetBufferMapping(BufferMapping mapping);

GLBufferImpl::GLBufferImpl(const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc)
{
	Create(bufferDesc, subresourceDesc);
}

GLBufferImpl::~GLBufferImpl()
{
	Destroy();
}

void GLBufferImpl::Create(const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc)
{
	Assert(bufferDesc.m_bufferMemorySize != 0);

	m_bufferDesc = bufferDesc;

	glGenBuffers(1, &m_handle);
	glNamedBufferDataEXT(m_handle, bufferDesc.m_bufferMemorySize, 
		subresourceDesc.m_memory, GetBufferAccess(bufferDesc.m_bufferAccess));
}

void GLBufferImpl::Destroy()
{
	glDeleteBuffers(1, &m_handle);
}

void* GLBufferImpl::Map(BufferMapping mapping)
{
	void* ret = glMapNamedBufferEXT(m_handle, GetBufferMapping(mapping));
	return ret;
}

void GLBufferImpl::Unmap()
{
	glUnmapNamedBufferEXT(m_handle);
}

void GLBufferImpl::UpdateSubresource(void* data, size_t size)
{
	glNamedBufferSubDataEXT(m_handle, 0, size, data);
}


}