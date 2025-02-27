#include "gldrv_pch.h"
#include "gldrv/gltexture2d.h"

namespace solunar
{

extern GLenum GetGLFormat(ImageFormat format);
extern GLenum GetGLInternalFormat(ImageFormat format);
extern GLenum GetGLTextureDataType(ImageFormat format);

GLTexture2D::GLTexture2D(const TextureDesc& textureDesc, const SubresourceDesc& subresourceDesc)
{
	Create(textureDesc, subresourceDesc);
}

GLTexture2D::~GLTexture2D()
{
	Destroy();
}

void GLTexture2D::Create(const TextureDesc& textureDesc, const SubresourceDesc& subresourceDesc)
{
	//ASSERT(textureDesc.m_textureType == TextureType::Texture2D);
	//ASSERT(subresourceDesc.m_memory);

	//////////////////////////
	// Initialize surface desc
	m_surfaceDesc.m_width = textureDesc.m_width;
	m_surfaceDesc.m_height = textureDesc.m_height;

	glGenTextures(1, &m_textureHandle);
	glTextureImage2DEXT(m_textureHandle, GL_TEXTURE_2D, 0, GetGLFormat(textureDesc.m_format),
		textureDesc.m_width, textureDesc.m_height, 0, GetGLInternalFormat(textureDesc.m_format),
		GetGLTextureDataType(textureDesc.m_format), subresourceDesc.m_memory);

	///////////////////////////////////
	// Initialize default sampler state
	glTextureParameteriEXT(m_textureHandle, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteriEXT(m_textureHandle, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteriEXT(m_textureHandle, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteriEXT(m_textureHandle, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void GLTexture2D::Destroy()
{
	if (m_textureHandle != 0)
		glDeleteTextures(1, &m_textureHandle);
}

SurfaceDesc GLTexture2D::getSurfaceDesc()
{
	return m_surfaceDesc;
}

void GLTexture2D::SetDebugName(const char* debugName)
{
}

void GLTexture2D::updateTexture(const void* data, int rowPitch, int depthPitch)
{
	Assert2(0, "Not implemented");
}

}