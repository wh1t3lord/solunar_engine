#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

namespace solunar
{
	class LightComponent;
	class PointLightComponent;
	class SpotLightComponent;
	class DirectionalLightComponent;

	class LightManager : public Singleton<LightManager>
	{
	public:
		void AddLight(LightComponent* light);
		void RemoveLight(LightComponent* light);

		std::vector<LightComponent*>& GetLights() { return m_lights; }
		std::vector<PointLightComponent*>& GetPointLights() { return m_pointLights; }
		std::vector<SpotLightComponent*>& GetSpotLights() { return m_spotLights; }

		DirectionalLightComponent* GetDirectionalLight();

	private:
		std::vector<LightComponent*> m_lights;
		std::vector<PointLightComponent*> m_pointLights;
		std::vector<SpotLightComponent*> m_spotLights;
		
	};

	void graphicsLightEditor(bool* open);
}

#endif // !LIGHTCONTROLLER_H
