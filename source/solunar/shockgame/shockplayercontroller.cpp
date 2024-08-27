#include "shockgamepch.h"
#include "shockgame/shockplayercontroller.h"

#include "engine/inputmanager.h"

#include "graphics/imguimanager.h"
#include "graphics/ifontmanager.h"
#include "graphics/animatedmodel.h"
#include "graphics/debugrenderer.h"

#include "game/gamelogic/weapons/weaponcomponent.h"

#include <numeric>
#include <limits>

namespace solunar
{

IMPLEMENT_OBJECT(ShockPlayerController, PlayerControllerComponent);

BEGIN_PROPERTY_REGISTER(ShockPlayerController)
{
	REGISTER_PROPERTY(ShockPlayerController, PropertyFloat, m_playerStats.m_health);
	REGISTER_PROPERTY(ShockPlayerController, PropertyFloat, m_playerStats.m_endurance);
}
END_PROPERTY_REGISTER(ShockPlayerController);

// #TODO: REFACTOR
Entity* g_freeCameraEntity = nullptr;
CameraFirstPersonComponent* g_freeCamera = nullptr;

ShockPlayerController::ShockPlayerController() :
	m_cameraEntity(nullptr),
	m_camera(nullptr),
	m_weaponEntity(nullptr),
	m_weaponMesh(nullptr),
	m_rigidBody(nullptr),
	m_flyCam(true)
{
	memset(&m_playerStats, 0, sizeof(m_playerStats));
	m_playerStats.m_health = 100.0f;
	m_playerStats.m_endurance = 25.0f;
	m_weaponSwayAngles = glm::vec3(0.0f);
}

ShockPlayerController::~ShockPlayerController()
{
}

void ShockPlayerController::RegisterObject()
{
	g_typeManager->RegisterObject<ShockPlayerController>();
}

void ShockPlayerController::OnEntitySet(Entity* entity)
{
	PlayerControllerComponent::OnEntitySet(entity);

	InitializeCamera();
	InitializeComponents();
}	

void ShockPlayerController::OnEntityRemove()
{
	// m_cameraNode = nullptr;
	// m_cameraTransform = nullptr;
	// m_camera = nullptr;
	// m_rigidBody = nullptr;

	PlayerControllerComponent::OnEntityRemove();
}

void ShockPlayerController::ActivateCamera()
{
	CameraProxy::GetInstance()->SetCameraComponent(m_camera);

	g_engineData.m_shouldCaptureMouse = true;
	g_engineData.m_shouldHideMouse = true;
}

void ShockPlayerController::InitializeCamera()
{
	// m_cameraNode = getNode()->CreateChild();
	// m_cameraTransform = m_cameraNode->createComponentByType<TransformComponent>();
	// m_camera = m_cameraNode->createComponentByType<CameraFirstPersonComponent>();

	m_cameraEntity = GetEntity()->CreateChild();
	m_cameraEntity->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));

	m_camera = m_cameraEntity->CreateComponent<CameraFirstPersonComponent>();
	//m_camera = getEntity()->createComponent<CameraFirstPersonComponent>();


	m_rigidBody = GetEntity()->CreateComponent<RigidBodyProxyComponent>();
	m_rigidBody->CreatePlayerController();

	ActivateCamera();

	// create weapon
	Entity* hackEntity = m_cameraEntity->CreateChild();
	//Entity* hackEntity = getEntity()->createChild();
	hackEntity->QuaternionRotate(glm::vec3(0.0f, 1.0f, 0.0f), -90.0f);

	m_weaponEntity = hackEntity->CreateChild();
	m_weaponEntity->SetPosition(glm::vec3(0.2f, -0.25f, -0.3f));

	// load model
	m_weaponMesh = m_weaponEntity->CreateComponent<AnimatedMeshComponent>();
	m_weaponMesh->LoadModel("models/viewmodel_shotgun.glb");

	m_weaponEntity->CreateComponent<WeaponComponent>();

	std::weak_ptr<AnimatedModel> weaponModel = dynamicCastWeakPtr<AnimatedModel, ModelBase>(m_weaponMesh->getModel());
	//int rootNodeIndex = weaponModel.lock()->getNodeByName("Root Node");
	//weaponModel.lock()->setNodeScale(rootNodeIndex, glm::vec3(0.1f));

	// little hack #TODO: please remove ViewmodelAnimationController from shockgame.cpp
	//Component* viewmodelComponent = (Component*)TypeManager::getInstance()->createObjectByName("ViewmodelAnimationController");
	//m_weaponEntity->addComponent(viewmodelComponent);

	// #TODO: REMOVE FROM THIS
	if (!g_freeCamera)
	{
		g_freeCameraEntity = GetWorld()->CreateEntity();
		g_freeCamera = g_freeCameraEntity->CreateComponent<CameraFirstPersonComponent>();
	}
}

void ShockPlayerController::InitializeComponents()
{
	// m_transform = getNode()->getComponentByTypeSafe<TransformComponent>();
	// m_rigidBody = getNode()->getComponentByTypeSafe<RigidBodyComponent>();
}

static IFont* s_font = nullptr;

void ShockPlayerController::Update(float dt)
{
	// if (m_rigidBody == nullptr)
	// {
	// 	initializeComponents();
	// 	//ASSERT2(m_rigidBody, "Node don't have rigid body component.");
	// }

		// #TODO: REMOVE FROM THIS
	if (InputManager::GetInstance()->IsPressedWithReset(KeyboardKeys::KEY_F2))
	{
		ActivateCamera();
	}

	if (InputManager::GetInstance()->IsPressedWithReset(KeyboardKeys::KEY_F3))
	{
		CameraProxy::GetInstance()->SetCameraComponent(g_freeCamera);
		g_freeCameraEntity->SetPosition(GetEntity()->GetWorldPosition());
	}

	if (CameraProxy::GetInstance()->GetCameraComponent() == g_freeCamera)
	{
		InputManager* input = InputManager::GetInstance();
		glm::vec2 mousePos = input->GetCursorPos();

#if 0
		glm::vec2 deltaMousePos = input->GetDeltaCursorPos();

		g_freeCamera->updateFromMousePosition(deltaMousePos);
#else
		static glm::vec2 deltaMousePos = glm::vec2(0.0f);

		deltaMousePos += input->GetDeltaCursorPos();

		g_freeCamera->updateFromMousePosition(deltaMousePos);
		input->ResetDelta();
#endif

		glm::vec3 cameraDirection = CameraProxy::GetInstance()->GetDirection();
		glm::vec3 pos = g_freeCameraEntity->GetPosition();
		float camSpeed = 8.0f * dt;

		if (input->IsPressed(KeyboardKeys::KEY_LEFT_SHIFT))
			camSpeed = 18.0f * dt;

		if (input->IsPressed(KeyboardKeys::KEY_UP))
			pos += camSpeed * cameraDirection;
		if (input->IsPressed(KeyboardKeys::KEY_DOWN))
			pos -= camSpeed * cameraDirection;

		//if (input->IsPressed(KeyboardKeys::KEY_Q))
		//	pos -= glm::normalize(glm::cross(cameraDirection, glm::vec3(1.0f, 0.0f, 0.0f))) * camSpeed;
		//if (input->IsPressed(KeyboardKeys::KEY_E))
		//	pos += glm::normalize(glm::cross(cameraDirection, glm::vec3(1.0f, 0.0f, 0.0f))) * camSpeed;

		if (input->IsPressed(KeyboardKeys::KEY_LEFT))
			pos -= glm::normalize(glm::cross(cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f))) * camSpeed;
		if (input->IsPressed(KeyboardKeys::KEY_RIGHT))
			pos += glm::normalize(glm::cross(cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f))) * camSpeed;

		g_freeCameraEntity->SetPosition(pos);
	}

	if (!s_font)
		s_font = g_fontManager->CreateFont("textures/ui/RobotoMono-Bold.ttf", 32.0f);

	g_engineData.m_shouldCaptureMouse = true;
	//g_engineData.m_shouldHideMouse = false;

#if 0
	// set position
	static glm::vec3 s_weaponOffset = glm::vec3(0.0f);
	ImGui::DragFloat3("", glm::value_ptr(s_weaponOffset), 0.1, -4.0f, 4.0f);

	m_weaponEntity->SetPosition(s_weaponOffset);
#endif

	View* view = CameraProxy::GetInstance()->GetView();
	
	if (m_playerStats.m_health <= 0.0f)
	{
		char healthText[64];
		snprintf(healthText, sizeof(healthText), "WE ARE DEAD :((((");
		g_fontManager->DrawSystemFont(healthText, 500, 500, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));
	}

#if 1
	char healthText[64];
	snprintf(healthText, sizeof(healthText), "Health: %.0f", m_playerStats.m_health);
	
	char enduranceText[64];
	snprintf(enduranceText, sizeof(enduranceText), "Endurance: %.0f", m_playerStats.m_endurance);

	s_font->DrawText(healthText, 25.0f, view->m_height - 50.0f, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));
	//s_font->DrawText(enduranceText, 25.0f, view->m_height - 25.0f, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));
#endif

	// update camera look
	UpdateCamera(dt);

	// update player movement
	UpdateMovement(dt);

	// update debug
	DebugUpdate(dt);

	if (InputManager::GetInstance()->IsPressedWithReset(KeyboardKeys::KEY_F1))
	{
		GetWorld()->TogglePhysicsDebugDraw();
	}
}

void ShockPlayerController::doHit(float amount)
{
	if (m_playerStats.m_health <= 0.0f)
		return; // we are dead

	m_playerStats.m_health -= amount;
}

void ShockPlayerController::UpdateCamera(float dt)
{
	 InputManager* input = InputManager::GetInstance();
	 glm::vec2 mousePos = input->GetCursorPos();

#if 0
	 glm::vec2 deltaMousePos = input->GetDeltaCursorPos();

	 m_camera->updateFromMousePosition(deltaMousePos);
#else
	 static glm::vec2 deltaMousePos = glm::vec2(0.0f);

	 deltaMousePos += input->GetDeltaCursorPos();

	 m_camera->updateFromMousePosition(deltaMousePos);
	 input->ResetDelta();
#endif

	glm::quat rot = glm::eulerAngleYX(glm::radians(-m_camera->m_yaw), glm::radians(m_camera->m_pitch));
	m_weaponEntity->SetRotation(rot);

#if 0
	 glm::vec3 cameraDirection = CameraProxy::GetInstance()->GetDirection();
	 glm::vec3 pos = GetEntity()->GetPosition();
	 float camSpeed = 8.0f * dt;

	 if (input->getKey(KeyboardKeys::KEY_LEFT_SHIFT))
		 camSpeed = 18.0f * dt;
	 
	 if (input->getKey(KeyboardKeys::KEY_W))
	 	pos += camSpeed * cameraDirection;
	 if (input->getKey(KeyboardKeys::KEY_S))
	 	pos -= camSpeed * cameraDirection;
	 
	 if (input->getKey(KeyboardKeys::KEY_A))
	 	pos -= glm::normalize(glm::cross(cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f))) * camSpeed;
	 if (input->getKey(KeyboardKeys::KEY_D))
	 	pos += glm::normalize(glm::cross(cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f))) * camSpeed;
	 
	 GetEntity()->SetPosition(pos);
#endif
}

void ShockPlayerController::UpdateMovement(float dt)
{
	 glm::vec3 moveVector = glm::vec3(0.0f);
	 
	 InputManager* inputManager = InputManager::GetInstance();
	 
	 bool isPlayerMove = (inputManager->IsPressed(KeyboardKeys::KEY_W)) ||
	 					(inputManager->IsPressed(KeyboardKeys::KEY_S)) ||
	 					(inputManager->IsPressed(KeyboardKeys::KEY_A)) ||
	 					(inputManager->IsPressed(KeyboardKeys::KEY_D));
	 
	 Camera* camera = CameraProxy::GetInstance();
	 
#if 0
	 m_onTheGround = true;
	 if (isPlayerMove && m_onTheGround)
	 {
		 glm::vec3 dir = glm::vec3(0.0f);
		 if (inputManager->getKey(KeyboardKeys::KEY_W))
			 dir += camera->GetDirection();
		 if (inputManager->getKey(KeyboardKeys::KEY_S))
			 dir -= camera->GetDirection();
		 if (inputManager->getKey(KeyboardKeys::KEY_A))
			 dir -= glm::normalize(glm::cross(camera->GetDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));
		 if (inputManager->getKey(KeyboardKeys::KEY_D))
			 dir += glm::normalize(glm::cross(camera->GetDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));

		 // apply impulse to rigid body
		 //m_rigidBody->ApplyImpulse(dir);
		 //m_rigidBody->SetDirection(glm::normalize(dir) * dt * 2.0f);
		 //m_rigidBody->SetPositionForce(getEntity()->getPosition());
		 m_rigidBody->GetCharacterController()->setVelocityForTimeInterval(
			 glmVectorToBt(glm::normalize(dir) * 2.0f),
			 2.0f);
	 }

	 if (!isPlayerMove)
		 m_rigidBody->GetCharacterController()->setVelocityForTimeInterval(
			 glmVectorToBt(glm::vec3(0.0f)),
			 2.0f);
#else
	 m_onTheGround = m_rigidBody->GetCharacterController()->onGround();
	 if (isPlayerMove && m_onTheGround)
	 {
		 glm::vec3 camdir = camera->GetDirection();
		 camdir.y = 0.0f;

		 glm::vec3 dir = glm::vec3(0.0f);
		 if (inputManager->IsPressed(KeyboardKeys::KEY_W))
			 dir += camdir;
		 if (inputManager->IsPressed(KeyboardKeys::KEY_S))
			 dir -= camdir;
		 if (inputManager->IsPressed(KeyboardKeys::KEY_A))
			 dir -= glm::normalize(glm::cross(camdir, glm::vec3(0.0f, 1.0f, 0.0f)));
		 if (inputManager->IsPressed(KeyboardKeys::KEY_D))
			 dir += glm::normalize(glm::cross(camdir, glm::vec3(0.0f, 1.0f, 0.0f)));

		 // apply impulse to rigid body
		 //m_rigidBody->ApplyImpulse(dir);
		 if (glm::length(dir) >= 0.01f)
			m_rigidBody->SetDirection(glm::normalize(dir) * dt * 2.0f);
		 //m_rigidBody->SetPositionForce(getEntity()->getPosition());
		
	 }

	 if (!isPlayerMove)
		 m_rigidBody->SetDirection(glm::vec3(0.0f));

	 // #TODO: Save jump velocity vector
	 if (inputManager->IsPressed(KeyboardKeys::KEY_SPACE) && m_onTheGround)
	 {
		 const glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
		 const float jumpPower = 6.0f;

		 glm::vec3 cameraDirection = camera->GetDirection();
		 cameraDirection.y = 0.0f;
		 
		 m_rigidBody->GetCharacterController()->jump(glmVectorToBt((upVector + cameraDirection) * jumpPower));
	 }

#endif

	 m_rigidBody->Update(dt);

	 btTransform trans = m_rigidBody->GetGhostObject()->getWorldTransform();
	 GetEntity()->SetPosition(btVectorToGlm(trans.getOrigin()));
}

void ShockPlayerController::DebugUpdate(float dt)
{
	return;

	if (!m_rigidBody)
		return;

	static char buf[256];
	
	// Ghost object stuff ...

	snprintf(buf, sizeof(buf), "--- Ghost Object ---");
	g_fontManager->DrawSystemFontShadowed(buf, 0, 200, glm::vec4(1.0f));

	btTransform trans = m_rigidBody->GetGhostObject()->getWorldTransform();
	
	glm::vec3 ghostObjectOrigin = btVectorToGlm(trans.getOrigin());
	snprintf(buf, sizeof(buf), "Origin: %.2f %.2f %.2f", 
		ghostObjectOrigin.x, 
		ghostObjectOrigin.y, 
		ghostObjectOrigin.z);

	g_fontManager->DrawSystemFontShadowed(buf, 0, 220, glm::vec4(1.0f));

	btQuaternion ghostObjectRotation = trans.getRotation();
	snprintf(buf, sizeof(buf), "Basis: %.2f %.2f %.2f %.2f", 
		ghostObjectRotation.x(),
		ghostObjectRotation.y(),
		ghostObjectRotation.z(),
		ghostObjectRotation.w());

	g_fontManager->DrawSystemFontShadowed(buf, 0, 240, glm::vec4(1.0f));

	snprintf(buf, sizeof(buf), "--- Character Controller ---");
	g_fontManager->DrawSystemFontShadowed(buf, 0, 280, glm::vec4(1.0f));

	snprintf(buf, sizeof(buf), "Delta: %f", dt);
	g_fontManager->DrawSystemFontShadowed(buf, 0, 300, glm::vec4(1.0f));

	btVector3 linearVelocity = m_rigidBody->GetCharacterController()->getLinearVelocity();

	snprintf(buf, sizeof(buf), "Linear velocity: %.2f %.2f %.2f",
		ghostObjectRotation.x(),
		ghostObjectRotation.y(),
		ghostObjectRotation.z());

	g_fontManager->DrawSystemFontShadowed(buf, 0, 320, glm::vec4(1.0f));

	btVector3 gravity = m_rigidBody->GetCharacterController()->getGravity();
	snprintf(buf, sizeof(buf), "Gravity: %.2f %.2f %.2f",
		ghostObjectRotation.x(),
		ghostObjectRotation.y(),
		ghostObjectRotation.z());

	g_fontManager->DrawSystemFontShadowed(buf, 0, 340, glm::vec4(1.0f));

	btScalar fallSpeed = m_rigidBody->GetCharacterController()->getFallSpeed();
	snprintf(buf, sizeof(buf), "fallSpeed: %f", fallSpeed);
	g_fontManager->DrawSystemFontShadowed(buf, 0, 360, glm::vec4(1.0f));

	bool onGround = m_rigidBody->GetCharacterController()->onGround();
	snprintf(buf, sizeof(buf), "onGround: %s", onGround ? "true" : "false");
	g_fontManager->DrawSystemFontShadowed(buf, 0, 380, glm::vec4(1.0f));
}

void shockGamePlayerDebug(bool* open)
{
	if (ImGui::Begin("Shock Player Debug", open))
	{
		World* world = Engine::ms_world;
		if (world)
		{
			std::vector<Entity*> players = world->GetEntityManager().GetEntitiesWithComponent<ShockPlayerController>();
			if (!players.empty())
			{
				Entity* player = players[0];
				ShockPlayerController* playerController = player->GetComponent<ShockPlayerController>();
				if (playerController)
				{
					ImGui::Checkbox("Fly cam", &playerController->m_flyCam);
				}
			}
			else
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Player with ShockPlayerController doesn't exist!");
			}
		}
		else
		{
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "World is not loaded!");
		}
	}

	ImGui::End();

}

}