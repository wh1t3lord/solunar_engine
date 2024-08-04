#ifndef GRAPHICS_GL_GLRENDERTARGET_H
#define GRAPHICS_GL_GLRENDERTARGET_H

#include "graphics/core/rendertarget.h"

namespace engine {

//! OpenGL implementation of IRenderTarget
class GLRenderTarget : public IRenderTarget
{
public:
	GLRenderTarget(const RenderTargetCreationDesc& renderTargetDesc);
	~GLRenderTarget();

	void create(const RenderTargetCreationDesc& renderTargetDesc);
	void release() override;

	uint32_t getHandle();

	void setDebugName(const char* debugName) override;

	// Old stuff of IRenderTarget, can be used ;)
	void attachTexture2D(int slot, ITexture2D* textureHandle);
	void attachDepth(ITexture2D* depthTextureHandle);

private:
	uint32_t m_handle;
};

}

#endif // !GRAPHICS_GL_GLRENDERTARGET_H
