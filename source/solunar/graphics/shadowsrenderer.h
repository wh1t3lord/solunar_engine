#ifndef SHADOWSRENDERER_H
#define SHADOWSRENDERER_H

#include "graphics/gfxcommon.h"

namespace solunar
{
	class IRenderTarget;
	class IRenderTarget;
	class ITexture2D;
	class IShaderProgram;

	class ShadowsRenderer : public Singleton<ShadowsRenderer>
	{
		static ShadowsRenderer ms_instance;
	public:
		ShadowsRenderer();
		~ShadowsRenderer();

		void Init();
		void Shutdown();

		void beginRender();
		void renderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh);
		void endRender();

		ITexture2D*			getTexture()				{ return m_shadowMap; }
		IShaderProgram*		getShader_StaticMesh()		{ return m_shadowShader_StaticMesh; }
		IShaderProgram*		getShader_AnimationMesh()	{ return m_shadowShader_AnimationMesh; }

	private:
		ITexture2D* m_shadowMap;
		IRenderTarget* m_shadowFbo;
		IShaderProgram* m_shadowShader_StaticMesh;
		IShaderProgram* m_shadowShader_AnimationMesh;

		Viewport m_originalViewport;
	};
}

#endif // !SHADOWSRENDERER_H
