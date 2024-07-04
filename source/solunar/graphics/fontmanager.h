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
class IRasterizerState;

struct FontVertex
{
	glm::vec2 position;
	glm::vec2 texcoord;
	glm::vec4 color;
};

// Limited to 1024 characters per one sentence
const int kMaxFontVBSize = sizeof(FontVertex)	* 1024;
const int kMaxFontIBSize = sizeof(uint32_t)		* 1024;

class FontManager : public IFontManager
{
public:
	FontManager();
	~FontManager();
	
	void initialize() override;
	void shutdown() override;

	void drawSystemFont(const char* text, int x, int y, const glm::vec4& color) override;

	void flushPrimitives() override;

private:
	void initPrivate();

private:
	struct SystemStringDrawInfo
	{
		std::string m_string;
		float m_x;
		float m_y;
		glm::vec4 m_color;
	};

	std::vector<SystemStringDrawInfo> m_systemDrawStrings;

	// System font
	stbtt_bakedchar m_systemFontChars[96];
	ITexture2D* m_systemFontTexture;
	
	// Shared font data
	ISamplerState* m_textureSampler;
	IBufferBase* m_vertexBuffer;
	IBufferBase* m_indexBuffer;
	IBufferBase* m_constantBuffer;
	IShaderProgram* m_shaderProgram;
	IRasterizerState* m_rasterizerState;

};

}

#endif // !GRAPHICS_FONTMANAGER_H
