#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

namespace engine
{
	class LightComponent;
	class DirectionalLightComponent;

	class LightManager 
	{
	public:
		void addLight(LightComponent* light);
		void removeLight(LightComponent* light);

		std::vector<LightComponent*>& getLights() { return m_lights; }

		DirectionalLightComponent* getDirectionalLight();

	private:
		std::vector<LightComponent*> m_lights;
	};
}

#endif // !LIGHTCONTROLLER_H
