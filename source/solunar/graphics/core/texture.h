#ifndef GRAPHICS_CORE_TEXTURE_H
#define GRAPHICS_CORE_TEXTURE_H

namespace solunar
{

class ITexture
{
public:
	virtual ~ITexture() {}

	virtual void updateTexture(const void* data, int rowPitch, int depthPitch) = 0;

	virtual SurfaceDesc getSurfaceDesc() = 0;

	//! Set debug name.
	virtual void setDebugName(const char* debugName) = 0;
};

class ITexture2D : public ITexture
{
public:
	//virtual ImageFormat getImageFormat() = 0;
};

class ISamplerState
{
public:
	virtual ~ISamplerState() {}

	//virtual SamplerDesc& getSamplerDesc() = 0;
};

}

#endif // !GRAPHICS_CORE_TEXTURE_H
