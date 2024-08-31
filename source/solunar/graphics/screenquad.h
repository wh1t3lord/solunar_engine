#ifndef SCREENQUAD_H
#define SCREENQUAD_H

namespace solunar
{
	class ITexture2D;
	class IShaderProgram;

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec2 texcoord;
	};

	class ScreenQuad
	{
	public:
		static InputLayoutDesc ms_inputLayout[2];

	private:
		static IBufferBase* ms_vertexBuffer;
		static IShaderProgram* ms_screenQuadShader;

	public:
		static void Init();
		static void Shutdown();

		static void Render(ITexture2D* texture);

		// same as upper but with custom shader.
		static void Render(ITexture2D* texture, IShaderProgram* shader);

		// same as upper but without shader binding.
		static void RenderWithoutShaderBinding(ITexture2D* texture);

		// same as upper but without texture binding.
		static void RenderWithoutTextureBinding(IShaderProgram* shader);

		// same as upper but without shader and texture binding. (just render screen quad)
		static void RenderWithoutShaderAndTextureBinding();
	};

}

#endif // !SCREENQUAD_H
