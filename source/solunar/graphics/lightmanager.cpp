#include "graphicspch.h"
#include "graphics/lightmanager.h"
#include "graphics/light.h"

namespace engine
{
	void LightManager::addLight(LightComponent* light)
	{
		Assert(light);
		m_lights.push_back(light);
	}

	void LightManager::removeLight(LightComponent* light)
	{
		Assert(light);
		
		typedef std::vector<LightComponent*>::iterator LT;

		LT I = m_lights.begin();
		LT E = m_lights.end();

		for (; I != E;)
			if (*I == light)
				break;

		m_lights.erase(I);
	}

	DirectionalLightComponent* LightManager::getDirectionalLight()
	{
		for (auto it : m_lights)
		{
			if (it->isA<DirectionalLightComponent>())
				return dynamicCast<DirectionalLightComponent>(it);
		}

		return nullptr;
	}
}