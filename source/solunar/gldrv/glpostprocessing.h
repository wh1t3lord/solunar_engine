#ifndef GRAPHICS_GL_GLPOSTPROCESSING_H
#define GRAPHICS_GL_GLPOSTPROCESSING_H

#include <gfxcommon.h>

namespace engine
{

class View;
class Texture2DBase;
class RenderTargetBase;
class SamplerStateBase;
class ShaderProgram;

class GLPostProcessing
{
public:
	GLPostProcessing();
	~GLPostProcessing();

	void init(View* view);
	void initBlur(View* view);

	void shutdown();

	void initHDR(View* view);
	void blurPass(Texture2DBase* screenTexture);
	void combinePass(Texture2DBase* screenTexture);

	void hdrPass(Texture2DBase* screenTexture);
private:
	TextureDesc m_hdrPingPongTextureDesc;

	Texture2DBase* m_hdrTempTex;
	RenderTargetBase* m_hdrRenderTarget;

	Texture2DBase* m_hdrPinPingTextures[2];
	RenderTargetBase* m_hdrPinPongRenderTargets[2];

	SamplerStateBase* m_hdrPinPongSampler;

	std::vector<glm::vec4> m_blurWeights;
	uint32_t m_blurWeightsLoc;

	glm::vec2 m_texOffset;

	ShaderProgram* m_hdrPassProgram;
	ShaderProgram* m_blurPassProgram;
	ShaderProgram* m_hdrCombineProgram;
};

extern GLPostProcessing g_postProcessing;

}

#endif // !GRAPHICS_GL_GLPOSTPROCESSING_H
