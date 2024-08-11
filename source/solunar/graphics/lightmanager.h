#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

namespace solunar
{
	class LightComponent;
	class PointLightComponent;
	class DirectionalLightComponent;

	class LightManager : public Singleton<LightManager>
	{
	public:
		void addLight(LightComponent* light);
		void removeLight(LightComponent* light);

		std::vector<LightComponent*>& GetLights() { return m_lights; }
		std::vector<PointLightComponent*>& GetPointLights() { return m_pointLights; }

		DirectionalLightComponent* GetDirectionalLight();

	private:
		std::vector<LightComponent*> m_lights;
		std::vector<PointLightComponent*> m_pointLights;
	};

	void graphicsLightEditor(bool* open);
}

#endif // !LIGHTCONTROLLER_H
