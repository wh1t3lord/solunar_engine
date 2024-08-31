#ifndef GRAPHICS_GL_GLPOSTPROCESSING_H
#define GRAPHICS_GL_GLPOSTPROCESSING_H

namespace solunar
{

class View;
class ITexture2D;
class IRenderTarget;
class ISamplerState;
class IShaderProgram;
class IBufferBase;

class GLPostProcessing
{
public:
	GLPostProcessing();
	~GLPostProcessing();

	void Init(View* view);
	void initBlur(View* view);

	void Shutdown();

	void initHDR(View* view);
	void blurPass(ITexture2D* screenTexture);
	void combinePass(ITexture2D* screenTexture);

	void HDRPass(ITexture2D* screenTexture);
private:
	TextureDesc m_hdrPingPongTextureDesc;

	ITexture2D* m_hdrTempTex;
	IRenderTarget* m_hdrRenderTarget;

	ITexture2D* m_hdrPinPingTextures[2];
	IRenderTarget* m_hdrPinPongRenderTargets[2];

	IBufferBase* m_hdrConstantBuffer;

	ISamplerState* m_hdrPinPongSampler;

	std::vector<glm::vec4> m_blurWeights;
	uint32_t m_blurWeightsLoc;

	glm::vec2 m_texOffset;

	IShaderProgram* m_hdrPassProgram;
	IShaderProgram* m_blurPassProgram;
	IShaderProgram* m_hdrCombineProgram;
};

extern GLPostProcessing g_postProcessing;

}

#endif // !GRAPHICS_GL_GLPOSTPROCESSING_H
