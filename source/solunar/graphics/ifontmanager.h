#ifndef GRAPHICS_IFONTMANAGER_H
#define GRAPHICS_IFONTMANAGER_H

namespace engine
{

class IFontManager
{
public:
	virtual void initialize() = 0;
	virtual void shutdown() = 0;

	virtual void drawSystemFont(const char* text, int x, int y) = 0;
};

extern IFontManager* g_fontManager;


}

#endif