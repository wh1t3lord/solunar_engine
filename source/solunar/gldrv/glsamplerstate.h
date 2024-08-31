#ifndef GRAPHICS_GL_GLSAMPLERSTATE_H
#define GRAPHICS_GL_GLSAMPLERSTATE_H

#include "graphics/core/texture.h"

namespace solunar
{

class GLSamplerState : public ISamplerState
{
public:
	GLSamplerState(const SamplerDesc& samplerDesc);
	~GLSamplerState();

	uint32_t getHandle() { return m_handle; }

private:
	uint32_t m_handle;
};

}

#endif // !GRAPHICS_GL_GLSAMPLERSTATE_H
