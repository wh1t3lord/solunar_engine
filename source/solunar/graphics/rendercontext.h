#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

namespace solunar
{
	class RenderContext
	{
	public:
		int width;
		int height;
		glm::mat4 view;
		glm::mat4 proj;
		glm::mat4 model;

	private:
		static RenderContext* ms_instance;

	public:
		static void Init();
		static void Shutdown();

		static RenderContext& GetContext();
		static void SetContext(RenderContext& ctx);

	};
}

#endif