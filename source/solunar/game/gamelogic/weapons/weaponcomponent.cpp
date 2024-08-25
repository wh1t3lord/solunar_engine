#include "gamepch.h"
#include "weaponcomponent.h"

#include "engine/camera.h"
#include "engine/inputmanager.h"

#include "engine/audio/audiomanager.h"
#include "engine/audio/audiosource.h"

#include "graphics/animatedmodel.h"
#include "graphics/mesh.h"
#include "graphics/ifontmanager.h"
#include "graphics/debugrenderer.h"

namespace solunar
{
	IMPLEMENT_OBJECT(WeaponComponent, LogicComponent);
	
	WeaponComponent::WeaponComponent() :
		m_inited(false),
		m_ammo(0)
	{
		// Shotgun
		m_ammo = 8;
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
		static AudioSource* s_fireSound = nullptr;
		static IFont* s_font = nullptr;

		AnimatedMeshComponent* mesh = GetEntity()->GetComponent<AnimatedMeshComponent>();
		std::shared_ptr<ModelBase> modelBase = mesh->lockModel();
		AnimatedModel* animatedModel = dynamicCast<AnimatedModel>(modelBase.get());
		if (!m_inited) {
			s_fireSound = AudioManager::GetInstance()->CreateSource("sounds/sfx/weapons/shotgun_fire.wav");

			s_font = g_fontManager->CreateFont("textures/ui/Anton-Regular.ttf", 50.0f);

			m_idleAni = animatedModel->GetAnimationByName("idle");
			m_fireAni = animatedModel->GetAnimationByName("fire");

			animatedModel->PlayAnimation(m_idleAni, true);
			m_inited = true;
		}

		static std::vector< std::pair<glm::vec3, glm::vec3> > debugLines;

		Camera* camera = CameraProxy::GetInstance();

		bool isFireAniFinished = animatedModel->GetCurrentAnimationId() == m_fireAni && animatedModel->IsStoped();

		if (InputManager::GetInstance()->IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
			( isFireAniFinished || animatedModel->GetCurrentAnimationId() != m_fireAni) &&
			m_ammo > 0) {
			animatedModel->PlayAnimation(m_fireAni, false);
			
			if (s_fireSound->IsPlaying())
				s_fireSound->Stop();

			s_fireSound->Play();

			--m_ammo;

			int shellCount = 6;
			for (int i = 0; i < shellCount; i++)
			{
				float r = rand() % 10;
				r = r / 100;

				glm::vec3 rayStart = camera->GetPosition() + r + camera->GetDirection() ;
				glm::vec3 rayEnd = camera->GetPosition() + r + camera->GetDirection()  * 1000.0f;

				//debugLines.push_back(std::make_pair(rayStart, rayEnd));

				RayCastResult rq = {};
				if (GetWorld()->RayCast(rq, rayStart, rayEnd))
				{
					Entity* entity = rq.m_entity;
					Core::Msg("WeaponComponent::Update(): shot entity 0x%p", entity);

					debugLines.push_back(std::make_pair(rayStart, rq.m_hitPosition));

					g_debugRender.drawAxis(rq.m_hitPosition);
				}
			}
		}		

		for (auto& it : debugLines)
		{
			g_debugRender.DrawLine(it.first, it.second, glm::vec3(1.0f, 0.0f, 0.0f));
		}

		if (isFireAniFinished) {
			animatedModel->PlayAnimation(m_idleAni, true);
		}

		animatedModel->Update(dt);

		View* view = CameraProxy::GetInstance()->GetView();

		char buf[256];

		snprintf(buf, sizeof(buf), "Ammo: %i", m_ammo);
		s_font->DrawText(buf, 25.0f, view->m_height - 20.0f, glm::vec4(1.0f, 0.2f, 0.0f, 1.0f));

#if 1
		snprintf(buf, sizeof(buf), "--- Viewmodel ---");
		g_fontManager->DrawSystemFontShadowed(buf, (float)view->m_width - 300.0f, 100.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		snprintf(buf, sizeof(buf), "Animation: %s", animatedModel->GetCurrentAnimation() ? animatedModel->GetCurrentAnimation()->m_name.c_str() : "NO ANIMATION");
		g_fontManager->DrawSystemFontShadowed(buf, (float)view->m_width - 300.0f, 125.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		snprintf(buf, sizeof(buf), "Time: %.2f", animatedModel->GetCurrentTime());
		g_fontManager->DrawSystemFontShadowed(buf, (float)view->m_width - 300.0f, 140.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
#endif
	}
}
