#ifndef GRAPHICS_GL_GLTEXTURE2D_H
#define GRAPHICS_GL_GLTEXTURE2D_H

#include "graphics/core/texture.h"

namespace solunar
{

class GLTexture2D : public ITexture2D
{
public:
	GLTexture2D(const TextureDesc& textureDesc, const SubresourceDesc& subresourceDesc);
	~GLTexture2D();

	void Create(const TextureDesc& textureDesc, const SubresourceDesc& subresourceDesc);
	void Destroy();

	void updateTexture(const void* data, int rowPitch, int depthPitch) override;

	SurfaceDesc getSurfaceDesc() override;

	uint32_t getHandle() { return m_textureHandle; }

	void setDebugName(const char* debugName) override;

private:
	SurfaceDesc m_surfaceDesc;
	uint32_t m_textureHandle;

};

}

#endif // !GRAPHICS_GL_GLTEXTURE2D_H
