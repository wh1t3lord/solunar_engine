#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

namespace engine
{
	struct RenderContext
	{
		int width;
		int height;
		glm::mat4 view;
		glm::mat4 proj;
		glm::mat4 model;
	};
}

#endif