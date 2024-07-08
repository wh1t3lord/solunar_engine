#ifndef GRAPHICS_IFONTMANAGER_H
#define GRAPHICS_IFONTMANAGER_H

namespace engine
{

class IFontManager
{
public:
	virtual void initialize() = 0;
	virtual void shutdown() = 0;

	virtual void drawSystemFont(const char* text, int x, int y, const glm::vec4& color) = 0;
	inline void drawSystemFontShadowed(const char* text, int x, int y, const glm::vec4& color);

	virtual void flushPrimitives() = 0;
};

extern IFontManager* g_fontManager;

inline void IFontManager::drawSystemFontShadowed(const char* text, int x, int y, const glm::vec4& color)
{
	drawSystemFont(text, x + 2, y + 2, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	drawSystemFont(text, x, y, color);
}

}

#endif