#ifndef SHADOWSRENDERER_H
#define SHADOWSRENDERER_H

#include "graphics/gfxcommon.h"

namespace engine
{

	class GrFramebuffer;
	class IRenderTarget;
	class ITexture2D;
	class IShaderProgram;

	class ShadowsRenderer : public Singleton<ShadowsRenderer>
	{
		static ShadowsRenderer ms_instance;
	public:
		ShadowsRenderer();
		~ShadowsRenderer();

		void init();
		void shutdown();

		void beginRender();
		void endRender();

		ITexture2D* getShadowMapTexture() { return m_shadowMap; }

		IShaderProgram* getShadowMapShader() { return m_shadowShader; }

	private:
		ITexture2D* m_shadowMap;
		GrFramebuffer* m_shadowFbo;
		IShaderProgram* m_shadowShader;

		Viewport m_originalViewport;
	};
}

#endif // !SHADOWSRENDERER_H
