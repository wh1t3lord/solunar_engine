#ifndef SHADOWSRENDERER_H
#define SHADOWSRENDERER_H

#include "graphics/gfxcommon.h"

namespace solunar
{
	class IRenderTarget;
	class IRenderTarget;
	class ITexture2D;
	class IShaderProgram;
	class ISamplerState;

	class ShadowsRenderer : public Singleton<ShadowsRenderer>
	{
		static ShadowsRenderer ms_instance;
	public:
		ShadowsRenderer();
		~ShadowsRenderer();

		void Init();
		void Shutdown();

		void BeginRender();
		void RenderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh);
		void EndRender();

		ITexture2D*			GetTexture()				{ return m_shadowMap; }
		ISamplerState*		GetSamplerState()			{ return m_shadowMapSampler; }
		IShaderProgram*		getShader_StaticMesh()		{ return m_shadowShader_StaticMesh; }
		IShaderProgram*		getShader_AnimationMesh()	{ return m_shadowShader_AnimationMesh; }

		const glm::mat4&	GetLightViewMatrix()		{ return m_lightViewMatrix; }
		const glm::mat4&	GetLightViewProjection()	{ return m_lightViewProjectionMatrix; }

	private:
		ITexture2D* m_shadowMap;
		ISamplerState* m_shadowMapSampler;
		IRenderTarget* m_shadowFbo;
		IShaderProgram* m_shadowShader_StaticMesh;
		IShaderProgram* m_shadowShader_AnimationMesh;

		glm::mat4 m_lightViewMatrix;
		glm::mat4 m_lightViewProjectionMatrix;

		float m_znear;
		float m_zfar;

		Viewport m_originalViewport;
	};
}

#endif // !SHADOWSRENDERER_H
