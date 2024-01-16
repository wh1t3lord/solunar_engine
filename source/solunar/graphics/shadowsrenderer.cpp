#include "graphicspch.h"

#include "graphics/shadowsrenderer.h"
#include "graphics/core/texture.h"
#include "graphics/core/device.h"
#include "graphics/core/rendertarget.h"
#include "graphics/shaderprogram.h"
#include "graphics/ShaderProgramManager.h"

namespace engine
{
	ShadowsRenderer ShadowsRenderer::ms_instance;

	ShadowsRenderer::ShadowsRenderer()
	{
		m_shadowShader = nullptr;
		memset(&m_originalViewport, 0, sizeof(m_originalViewport));
	}

	ShadowsRenderer::~ShadowsRenderer()
	{
		m_shadowShader = nullptr;
		memset(&m_originalViewport, 0, sizeof(m_originalViewport));
	}

	int g_shadowMapSize = 512;

	void ShadowsRenderer::init()
	{
		return;

		TextureDesc textureDesc;
		memset(&textureDesc, 0, sizeof(textureDesc));
		textureDesc.m_textureType = TextureType::Texture2D;
		textureDesc.m_width = textureDesc.m_height = g_shadowMapSize;
		textureDesc.m_mipmapLevel = 1;
		textureDesc.m_format = ImageFormat::R32;

		SubresourceDesc subresourceDesc;
		memset(&subresourceDesc, 0, sizeof(subresourceDesc));


	/*	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			Core::error("ShadowsRenderer::init: Framebuffer is not complete.");
		}*/

		g_renderDevice->setRenderTarget(0);

		/*TextureCreationDesc shadowMapDesc;
		memset(&shadowMapDesc, 0, sizeof(shadowMapDesc));
		shadowMapDesc.m_width = shadowMapDesc.m_height = shadowMapSize;
		shadowMapDesc.m_format = ImageFormat::R32F;
		m_shadowMapTexture = GraphicsDevice::instance()->createTexture2D(shadowMapDesc);

		m_renderTarget = GraphicsDevice::instance()->createFramebuffer(shadowMapSize, shadowMapSize);
		GraphicsDevice::instance()->setFramebuffer(m_renderTarget);

		m_renderTarget->setColorTexture(0, m_shadowMapTexture);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			Core::error("ShadowsRenderer::init: framebuffer error");

		GraphicsDevice::instance()->setFramebuffer(0);

		m_shadowShader = g_shaderManager->createShaderProgram("shadowmap_nonskinned",
			"shaders/shadowmap.vsh",
			"shaders/shadowmap.fsh");*/
	}

	void ShadowsRenderer::shutdown()
	{

		/*GraphicsDevice::instance()->deleteFramebuffer(m_renderTarget);
		GraphicsDevice::instance()->deleteTexture2D(m_shadowMapTexture);*/
	}

	void ShadowsRenderer::beginRender()
	{
		/*GraphicsDevice* graphicsDevice = GraphicsDevice::instance();*/

		//m_originalViewport = graphicsDevice->getViewport();

		//// set framebuffer
		//graphicsDevice->setFramebuffer(m_renderTarget);

		//// install shadowmap viewport
		//Viewport vp;
		//vp.m_x = 0;
		//vp.m_y = 0;
		//vp.m_width = vp.m_height = shadowMapSize;
		//graphicsDevice->setViewport(&vp);
	}

	void ShadowsRenderer::endRender()
	{
		//GraphicsDevice* graphicsDevice = GraphicsDevice::instance();

		//// set framebuffer
		//graphicsDevice->setFramebuffer(0);

		//// restore viewport
		//graphicsDevice->setViewport(&m_originalViewport);
	}

}