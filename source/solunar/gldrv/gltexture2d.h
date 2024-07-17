#ifndef GRAPHICS_GL_GLTEXTURE2D_H
#define GRAPHICS_GL_GLTEXTURE2D_H

#include "graphics/core/texture.h"

namespace engine
{

class GLTexture2D : public ITexture2D
{
public:
	GLTexture2D(const TextureDesc& textureDesc, const SubresourceDesc& subresourceDesc);
	~GLTexture2D();

	void create(const TextureDesc& textureDesc, const SubresourceDesc& subresourceDesc);
	void destroy();

	SurfaceDesc getSurfaceDesc() override;

	uint32_t getHandle() { return m_textureHandle; }

private:
	SurfaceDesc m_surfaceDesc;
	uint32_t m_textureHandle;
};

}

#endif // !GRAPHICS_GL_GLTEXTURE2D_H
