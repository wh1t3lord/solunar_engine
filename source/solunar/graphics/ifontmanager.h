#ifndef GRAPHICS_IFONTMANAGER_H
#define GRAPHICS_IFONTMANAGER_H

namespace solunar
{

class IFont
{
public:
	virtual ~IFont() {}

	virtual void DrawText(const char* text, float x, float y, const glm::vec4& color) = 0;
};

class IFontManager
{
public:
	virtual ~IFontManager() {}

	virtual void Initialize() = 0;
	virtual void Shutdown() = 0;

	virtual IFont* CreateFont(const char* filename, float size) = 0;

	virtual void DrawFontText(IFont* font, const char* text, float x, float y, const glm::vec4& color) = 0;

	virtual void DrawSystemFont(const char* text, float x, float y, const glm::vec4& color) = 0;
	inline void DrawSystemFontShadowed(const char* text, float x, float y, const glm::vec4& color);

	virtual void FlushPrimitives() = 0;
};

extern IFontManager* g_fontManager;

inline void IFontManager::DrawSystemFontShadowed(const char* text, float x, float y, const glm::vec4& color)
{
	DrawSystemFont(text, x + 1.0f, y + 1.0f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	DrawSystemFont(text, x, y, color);
}

}

#endif