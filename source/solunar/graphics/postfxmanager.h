#ifndef GRAPHICS_GL_GLPOSTPROCESSING_H
#define GRAPHICS_GL_GLPOSTPROCESSING_H

#include "graphics/shaderconstantmanager.h"

namespace engine
{

class View;
class ITexture2D;
class IRenderTarget;
class ISamplerState;
class IShaderProgram;
struct IBlendState;

class PostFxManager
{
public:
	PostFxManager();
	~PostFxManager();

	void init(View* view);
	void initBlur(View* view);

	void shutdown();

	void initHDR(View* view);
	void blurPass(ITexture2D* screenTexture);
	void combinePass(ITexture2D* screenTexture);

	void hdrPass(ITexture2D* screenTexture);
private:
	TextureDesc m_hdrPingPongTextureDesc;

	ITexture2D* m_hdrTempTex;
	IRenderTarget* m_hdrRenderTarget;

	ITexture2D* m_hdrPinPongTextures[2];
	IRenderTarget* m_hdrPinPongRenderTargets[2];

	ISamplerState* m_hdrPinPongSampler;

	std::vector<glm::vec4> m_blurWeights;
	uint32_t m_blurWeightsLoc;

	glm::vec2 m_texOffset;

	IShaderProgram* m_hdrPassProgram;
	IShaderProgram* m_blurPassProgram;
	IShaderProgram* m_blurPassHorizontalProgram;
	IShaderProgram* m_hdrCombineProgram;

	IBlendState* m_blendState;

	ConstantBufferProxy m_hdrconstants;
};

extern PostFxManager g_postFxManager;

}

#endif // !GRAPHICS_GL_GLPOSTPROCESSING_H
