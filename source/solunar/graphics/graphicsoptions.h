#ifndef GRAPHICSSETTINGS_H
#define GRAPHICSSETTINGS_H

namespace solunar
{
	class GraphicsOptions
	{
	public:
		bool loadSettings(const std::string& filename);
		void saveSettings(const std::string& filename);

		void applyDefaultOptions();

	public:
		int m_width, m_height;
		int m_refreshRate;
		bool m_fullscreen;

		// texture budget settings
		int m_texturesMipMapLevel;
		//int m_worldTexturesMipMapLevel;
		//int m_charactersTexturesMipMapLevel;

		int m_anisotropicQuality;
		int m_shadowsQuality;

		int m_vsync;
	};

	extern GraphicsOptions g_graphicsOptions;
}

#endif // !GRAPHICSSETTINGS_H
