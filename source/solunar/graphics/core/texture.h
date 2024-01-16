#ifndef GRAPHICS_CORE_TEXTURE_H
#define GRAPHICS_CORE_TEXTURE_H

namespace engine
{

class ITexture
{
public:
	virtual ~ITexture() {}

	virtual SurfaceDesc getSurfaceDesc() = 0;
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
