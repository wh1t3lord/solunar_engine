#include "gamepch.h"
#include "weaponcomponent.h"

#include "engine/camera.h"
#include "engine/inputmanager.h"

#include "graphics/animatedmodel.h"
#include "graphics/mesh.h"
#include "graphics/ifontmanager.h"

namespace solunar
{
	WeaponComponent::WeaponComponent() :
		m_inited(false)
	{
	}

	WeaponComponent::~WeaponComponent()
	{
	}

	void WeaponComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<WeaponComponent>();
	}

	void WeaponComponent::Update(float dt)
	{
		AnimatedMeshComponent* mesh = getEntity()->getComponent<AnimatedMeshComponent>();
		std::shared_ptr<ModelBase> modelBase = mesh->lockModel();
		AnimatedModel* animatedModel = dynamicCast<AnimatedModel>(modelBase.get());
		if (!m_inited) {
			m_idleAni = animatedModel->getAnimationByName("idle");
			m_fireAni = animatedModel->getAnimationByName("fire");

			animatedModel->setPlayAnimation(m_idleAni, true);
			m_inited = true;
		}

		Camera* camera = CameraProxy::getInstance();

		bool isFireAniFinished = animatedModel->getCurrentAnimationId() == m_fireAni && animatedModel->isStoped();

		if (InputManager::getInstance()->isPressed(KeyboardKeys::KEY_F)) {
			animatedModel->setPlayAnimation(m_fireAni, false);

			RayCastResult rq = {};
			//if (getWorld()->rayCast(rq, camera->getDirection(), camera->getDirection() + glm::vec3(10000.0f)))
			//{
			//	
			//}
		}

		if (isFireAniFinished) {
			animatedModel->setPlayAnimation(m_idleAni, true);
		}

		animatedModel->testPlay(dt);

#if 1
		View* view = CameraProxy::getInstance()->getView();

		char buf[256];

		snprintf(buf, sizeof(buf), "--- Viewmodel ---");
		g_fontManager->drawSystemFontShadowed(buf, (float)view->m_width - 300.0f, 100.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		snprintf(buf, sizeof(buf), "Animation: %s", animatedModel->getCurrentAnimation() ? animatedModel->getCurrentAnimation()->m_name.c_str() : "NO ANIMATION");
		g_fontManager->drawSystemFontShadowed(buf, (float)view->m_width - 300.0f, 125.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		snprintf(buf, sizeof(buf), "Time: %.2f", animatedModel->getCurrentTime());
		g_fontManager->drawSystemFontShadowed(buf, (float)view->m_width - 300.0f, 140.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
#endif
	}
}
