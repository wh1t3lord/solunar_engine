#ifndef GRAPHICS_FONTMANAGER_H
#define GRAPHICS_FONTMANAGER_H

#include "graphics/ifontmanager.h"

#include "stb_truetype.h"

namespace engine
{

class ITexture2D;
class IBufferBase;
class IShaderProgram;
class ISamplerState;

class FontManager : public IFontManager
{
public:
	void initialize() override;
	void shutdown() override;

	void drawSystemFont(const char* text, int x, int y) override;

private:
	void initPrivate();

private:
	// System font
	stbtt_bakedchar m_systemFontChars[96];
	ITexture2D* m_systemFontTexture;
	
	// Shared font data
	ISamplerState* m_fontTextureSamplerState;
	IBufferBase* m_fontVertexBuffer;
	IShaderProgram* m_defaultShaderProgram;

};

}

#endif // !GRAPHICS_FONTMANAGER_H
