#ifndef GRAPHICS_GL_GLRENDERTARGET_H
#define GRAPHICS_GL_GLRENDERTARGET_H

#include "graphics/core/rendertarget.h"

namespace solunar {

//! OpenGL implementation of IRenderTarget
class GLRenderTarget : public IRenderTarget
{
public:
	GLRenderTarget(const RenderTargetCreationDesc& renderTargetDesc);
	~GLRenderTarget();

	void Create(const RenderTargetCreationDesc& renderTargetDesc);
	void Release() override;

	uint32_t getHandle();

	void SetDebugName(const char* debugName) override;

	// Old stuff of IRenderTarget, can be used ;)
	void attachTexture2D(int slot, ITexture2D* textureHandle);
	void attachDepth(ITexture2D* depthTextureHandle);

private:
	uint32_t m_handle;
};

}

#endif // !GRAPHICS_GL_GLRENDERTARGET_H
