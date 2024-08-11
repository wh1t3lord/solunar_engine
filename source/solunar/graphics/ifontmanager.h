#ifndef GRAPHICS_IFONTMANAGER_H
#define GRAPHICS_IFONTMANAGER_H

namespace solunar
{

class IFont
{
public:
	virtual ~IFont() {}

	virtual void drawText(const char* text, float x, float y, const glm::vec4& color) = 0;
};

class IFontManager
{
public:
	virtual ~IFontManager() {}

	virtual void initialize() = 0;
	virtual void shutdown() = 0;

	virtual IFont* createFont(const char* filename, float size) = 0;

	virtual void drawFontText(IFont* font, const char* text, float x, float y, const glm::vec4& color) = 0;

	virtual void drawSystemFont(const char* text, float x, float y, const glm::vec4& color) = 0;
	inline void drawSystemFontShadowed(const char* text, float x, float y, const glm::vec4& color);

	virtual void flushPrimitives() = 0;
};

extern IFontManager* g_fontManager;

inline void IFontManager::drawSystemFontShadowed(const char* text, float x, float y, const glm::vec4& color)
{
	drawSystemFont(text, x + 1.0f, y + 1.0f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	drawSystemFont(text, x, y, color);
}

}

#endif