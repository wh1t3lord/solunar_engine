#include "gldrv_pch.h"
#include "glad/glad.h"

namespace solunar
{

GLenum getBufferAccess(BufferAccess access)
{
	switch (access)
	{
	case BufferAccess::Static: return GL_STATIC_DRAW;
	case BufferAccess::Dynamic: return GL_DYNAMIC_DRAW;
	case BufferAccess::Stream: return GL_STREAM_DRAW;
	}

	return 0;
}

GLenum getBufferMapping(BufferMapping mapping)
{
	switch (mapping)
	{
	case BufferMapping::ReadOnly: return GL_READ_ONLY;
	case BufferMapping::WriteOnly: return GL_WRITE_ONLY;
	case BufferMapping::ReadAndWrite: return GL_READ_WRITE;
	}

	return 0;
}

GLenum GetGLFormat(ImageFormat format)
{
	switch (format)
	{

		//case ImageFormat::DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
		//case ImageFormat::DEPTH32_STENCIL_8_F: return GL_DEPTH32F_STENCIL8;

	case ImageFormat::R32: return GL_RED;
	case ImageFormat::R32F: return GL_R32F;

	case ImageFormat::RGB16: return GL_RGB16;
	case ImageFormat::RGBA16: return GL_RGBA16;
	case ImageFormat::RGB32: return GL_RGB;
	case ImageFormat::RGBA32: return GL_RGBA;

	case ImageFormat::RGB16F: return GL_RGB16F;
	case ImageFormat::RGBA16F: return GL_RGBA16F;
	case ImageFormat::RGB32F: return GL_RGB32F;
	case ImageFormat::RGBA32F: return GL_RGBA32F;

	case ImageFormat::DEPTH24: return GL_DEPTH_COMPONENT24;
	case ImageFormat::DEPTH32: return GL_DEPTH_COMPONENT32;
	case ImageFormat::DEPTH32F: return GL_DEPTH_COMPONENT32F;

		//case ImageFormat::SRGB: return GL_SRGB;
	}

	return 0;
}

GLenum GetGLInternalFormat(ImageFormat format)
{
	switch (format)
	{

		//case ImageFormat::FMT_SRGB: return GL_RGB;

		//case ImageFormat::FMT_DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
		//case ImageFormat::FMT_DEPTH32_STENCIL_8_F: return GL_DEPTH32F_STENCIL8;

	case ImageFormat::R32:
	case ImageFormat::R32F:
		return GL_RED;

	case ImageFormat::RGB16:
	case ImageFormat::RGB32:
	case ImageFormat::RGB16F:
	case ImageFormat::RGB32F:
		return GL_RGB;

	case ImageFormat::RGBA16:
	case ImageFormat::RGBA32:
	case ImageFormat::RGBA32F:
	case ImageFormat::RGBA16F:
		return GL_RGBA;

	case ImageFormat::DEPTH24:
	case ImageFormat::DEPTH32:
	case ImageFormat::DEPTH32F:
		return GL_DEPTH_COMPONENT;
	}

	return 0;
}

GLenum GetGLTextureDataType(ImageFormat format)
{
	switch (format)
	{
	
	case ImageFormat::R32:
	case ImageFormat::RGB16:
	case ImageFormat::RGBA16:
	case ImageFormat::RGB32:
	case ImageFormat::RGBA32:
	case ImageFormat::DEPTH24:
	case ImageFormat::DEPTH32:
		return GL_UNSIGNED_BYTE;

	case ImageFormat::R32F:
	case ImageFormat::RGB16F:
	case ImageFormat::RGBA16F:
	case ImageFormat::RGB32F:
	case ImageFormat::RGBA32F:
	case ImageFormat::DEPTH32F:
		return GL_FLOAT;
	}

	return 0;
}

GLint GetComponentsFromFormat(ImageFormat format)
{
	switch (format)
	{
	case ImageFormat::RGB16:
	case ImageFormat::RGB32:
	case ImageFormat::RGB16F:
	case ImageFormat::RGB32F:
		return 3;

	case ImageFormat::RGBA16:
	case ImageFormat::RGBA32:
	case ImageFormat::RGBA16F:
	case ImageFormat::RGBA32F:
		return 4;
	default:
		break;
	}

	return 0;
}

GLint getGlWrap(TextureWrap wrap)
{
	GLint param = 0;

	if (wrap == TextureWrap::Repeat)
		param = GL_REPEAT;
	else if (wrap == TextureWrap::MirroredRepeat)
		param = GL_MIRRORED_REPEAT;
	else if (wrap == TextureWrap::ClampToEdge)
		param = GL_CLAMP_TO_EDGE;
	else if (wrap == TextureWrap::ClampToBorder)
		param = GL_CLAMP_TO_BORDER;

	return param;
}

GLint getGlTexFilter(TextureFilter filter)
{
	GLint param = 0;

	if (filter == TextureFilter::Linear)
		param = GL_LINEAR;
	else if (filter == TextureFilter::Nearest)
		param = GL_NEAREST;
	else if (filter == TextureFilter::LinearMipmapLinear)
		param = GL_LINEAR_MIPMAP_LINEAR;
	else if (filter == TextureFilter::LinearMipmapNearest)
		param = GL_LINEAR_MIPMAP_NEAREST;
	else if (filter == TextureFilter::NearestMipmapLinear)
		param = GL_NEAREST_MIPMAP_LINEAR;
	else if (filter == TextureFilter::NearestMipmapNearest)
		param = GL_NEAREST_MIPMAP_NEAREST;

	return param;
}

}