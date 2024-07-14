#ifndef SCREENQUAD_H
#define SCREENQUAD_H

namespace engine
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
		static void init();
		static void shutdown();

		static void render(ITexture2D* texture);

		// same as upper but with custom shader.
		static void render(ITexture2D* texture, IShaderProgram* shader);

		// same as upper but without shader binding.
		static void renderWithoutShaderBinding(ITexture2D* texture);

		// same as upper but without texture binding.
		static void renderWithoutTextureBinding(IShaderProgram* shader);

		// same as upper but without shader and texture binding. (just render screen quad)
		static void renderWithoutShaderAndTextureBinding();
	};

}

#endif // !SCREENQUAD_H
