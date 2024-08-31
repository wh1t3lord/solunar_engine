#include "gldrv_pch.h"
#include "gldrv/glrendertarget.h"
#include "gldrv/gltexture2d.h"

namespace solunar {

	GLRenderTarget::GLRenderTarget(const RenderTargetCreationDesc& renderTargetDesc)
	{
		Create(renderTargetDesc);
	}

	GLRenderTarget::~GLRenderTarget()
	{
		m_handle = 0;
	}

	void GLRenderTarget::Create(const RenderTargetCreationDesc& renderTargetDesc)
	{
		Assert(renderTargetDesc.m_textures2DCount != 0);

		glGenFramebuffers(1, &m_handle);
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

		std::vector<GLenum> colorAttachments;

		// bind color targets
		for (int i = 0; i < renderTargetDesc.m_textures2DCount; i++)
		{
			Assert(renderTargetDesc.m_textures2D[i]);
			attachTexture2D(i, renderTargetDesc.m_textures2D[i]);
			colorAttachments.push_back(GL_COLOR_ATTACHMENT0 + i);
		}

		if (renderTargetDesc.m_depthTexture2D)
		{
			attachDepth(renderTargetDesc.m_depthTexture2D);
		}
		
		// Initialize draw bufferz
		glDrawBuffers(colorAttachments.size(), colorAttachments.data());

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			Core::Error("GLRenderTarget::create: Framebuffer is not complete.");
		}
	
		//unbind
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLRenderTarget::Release()
	{
		glDeleteFramebuffers(1, &m_handle);
	}

	uint32_t GLRenderTarget::getHandle()
	{
		return m_handle;
	}

	void GLRenderTarget::SetDebugName(const char* debugName)
	{
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
