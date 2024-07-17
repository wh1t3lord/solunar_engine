#ifndef GRAPHICS_GL_GLRENDERTARGET_H
#define GRAPHICS_GL_GLRENDERTARGET_H

#include "graphics/core/rendertarget.h"

namespace engine {

//! OpenGL implementation of IRenderTarget
class GLRenderTarget : public IRenderTarget
{
public:
	GLRenderTarget();
	~GLRenderTarget();

	void create();
	void release() override;

	uint32_t getHandle() override;

	void attachTexture2D(int slot, ITexture2D* textureHandle);
	void attachDepth(ITexture2D* depthTextureHandle);

private:
	uint32_t m_handle;
};

}

#endif // !GRAPHICS_GL_GLRENDERTARGET_H
