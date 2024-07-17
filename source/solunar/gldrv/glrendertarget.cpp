#include "gldrv_pch.h"
#include "gldrv/glrendertarget.h"
#include "gldrv/gltexture2d.h"


namespace engine {

	GLRenderTarget::GLRenderTarget()
	{
		create();
	}

	GLRenderTarget::~GLRenderTarget()
	{
		m_handle = 0;
	}

	void GLRenderTarget::create()
	{
		glGenFramebuffers(1, &m_handle);
	}

	void GLRenderTarget::release()
	{
		glDeleteFramebuffers(1, &m_handle);
	}

	uint32_t GLRenderTarget::getHandle()
	{
		return m_handle;
	}

	void GLRenderTarget::attachTexture2D(int slot, ITexture2D* textureHandle)
	{
		GLTexture2D* glTex = reinterpret_cast<GLTexture2D*>(textureHandle);
		glNamedFramebufferTextureEXT(m_handle, GL_COLOR_ATTACHMENT0 + slot, glTex->getHandle(), 0);
	}

	void GLRenderTarget::attachDepth(ITexture2D* depthTextureHandle)
	{
		GLTexture2D* glTex = reinterpret_cast<GLTexture2D*>(depthTextureHandle);
		glNamedFramebufferTextureEXT(m_handle, GL_DEPTH_ATTACHMENT, glTex->getHandle(), 0);
	}
}
