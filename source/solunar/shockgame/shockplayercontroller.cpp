#include "shockgamepch.h"
#include "shockgame/shockplayercontroller.h"

#include "engine/inputmanager.h"

#include "graphics/imguimanager.h"
#include "graphics/ifontmanager.h"

#include <numeric>
#include <limits>
#include "graphics/animatedmodel.h"

namespace engine
{

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
}

ShockPlayerController::~ShockPlayerController()
{
}

void ShockPlayerController::registerObject()
{
	g_typeManager->registerObject<ShockPlayerController>();
}

void ShockPlayerController::onEntitySet(Entity* entity)
{
	PlayerControllerComponent::onEntitySet(entity);

	initializeCamera();
	initializeComponents();
}	

void ShockPlayerController::onEntityRemove()
{
	// m_cameraNode = nullptr;
	// m_cameraTransform = nullptr;
	// m_camera = nullptr;
	// m_rigidBody = nullptr;

	PlayerControllerComponent::onEntityRemove();
}

void ShockPlayerController::activateCamera()
{
	CameraProxy::getInstance()->setCameraComponent(m_camera);
}

void ShockPlayerController::initializeCamera()
{
	// m_cameraNode = getNode()->createChild();
	// m_cameraTransform = m_cameraNode->createComponentByType<TransformComponent>();
	// m_camera = m_cameraNode->createComponentByType<CameraFirstPersonComponent>();

	m_camera = getEntity()->createComponent<CameraFirstPersonComponent>();

	m_rigidBody = getEntity()->createComponent<RigidBodyProxyComponent>();
	m_rigidBody->createPlayerController();

	activateCamera();

	// create weapon
	Entity* hackEntity = getEntity()->createChild();
	hackEntity->quaternionRotate(glm::vec3(0.0f, 1.0f, 0.0f), -90.0f);

	m_weaponEntity = hackEntity->createChild();
	m_weaponEntity->setPosition(glm::vec3(0.1f, -3.4f, -0.7f));

	// load model
	m_weaponMesh = m_weaponEntity->createComponent<AnimatedMeshComponent>();
	m_weaponMesh->loadModel("models/viewmodel_test.glb");

	std::weak_ptr<AnimatedModel> weaponModel = dynamicCastWeakPtr<AnimatedModel, ModelBase>(m_weaponMesh->getModel());
	int rootNodeIndex = weaponModel.lock()->getNodeByName("Root Node");
	weaponModel.lock()->setNodeScale(rootNodeIndex, glm::vec3(0.1f));

	// little hack #TODO: please remove ViewmodelAnimationController from shockgame.cpp
	Component* viewmodelComponent = (Component*)TypeManager::getInstance()->createObjectByName("ViewmodelAnimationController");
	m_weaponEntity->addComponent(viewmodelComponent);
}

void ShockPlayerController::initializeComponents()
{
	// m_transform = getNode()->getComponentByTypeSafe<TransformComponent>();
	// m_rigidBody = getNode()->getComponentByTypeSafe<RigidBodyComponent>();
}

void ShockPlayerController::update(float dt)
{
	// if (m_rigidBody == nullptr)
	// {
	// 	initializeComponents();
	// 	//ASSERT2(m_rigidBody, "Node don't have rigid body component.");
	// }

#if 0
	// set position
	static glm::vec3 s_weaponOffset = glm::vec3(0.0f);
	ImGui::DragFloat3("", glm::value_ptr(s_weaponOffset), 0.1, -4.0f, 4.0f);

	m_weaponEntity->setPosition(s_weaponOffset);
#endif

	View* view = CameraProxy::getInstance()->getView();
	
	if (m_playerStats.m_health <= 0.0f)
	{
		char healthText[64];
		snprintf(healthText, sizeof(healthText), "WE ARE DEAD :((((");
		g_fontManager->drawSystemFont(healthText, 500, 500, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));
	}

#if 1
	char healthText[64];
	snprintf(healthText, sizeof(healthText), "Health: %.0f", m_playerStats.m_health);
	
	char enduranceText[64];
	snprintf(enduranceText, sizeof(enduranceText), "Endurance: %.0f", m_playerStats.m_endurance);

	g_fontManager->drawSystemFont(healthText, 25, view->m_height - 50, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));
	g_fontManager->drawSystemFont(enduranceText, 25, view->m_height - 25, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));
#endif

	// update camera look
	updateCamera(dt);

	// update player movement
	updateMovement(dt);

	// update debug
	debugUpdate(dt);

	InputManager* input = InputManager::getInstance();
	if (input->getKey(KeyboardKeys::KEY_F))
	{
		Camera* camera = CameraProxy::getInstance();
		glm::vec3 rayStart = camera->getPosition() + camera->getDirection();
		glm::vec3 rayEnd = rayStart + 1000.0f;

		RayCastResult rq = {};
		if (getWorld()->rayCast(rq, rayStart, rayEnd))
		{
			Entity* entity = rq.m_entity;
			Core::msg("ShockPlayerController::update(): looking at entity 0x%p", entity);
		}
	}

	if (input->getKeyWithReset(KeyboardKeys::KEY_F1))
	{
		getWorld()->togglePhysicsDebugDraw();
	}
}

void ShockPlayerController::doHit(float amount)
{
	if (m_playerStats.m_health <= 0.0f)
		return; // we are dead

	m_playerStats.m_health -= amount;
}

void ShockPlayerController::updateCamera(float dt)
{
	 InputManager* input = InputManager::getInstance();
	 glm::vec2 mousePos = input->getCursorPos();

#if 0
	 glm::vec2 deltaMousePos = input->getDeltaCursorPos();

	 m_camera->updateFromMousePosition(deltaMousePos);
#else
	 static glm::vec2 deltaMousePos = glm::vec2(0.0f);

	 deltaMousePos += input->getDeltaCursorPos();

	 m_camera->updateFromMousePosition(deltaMousePos);
	 input->resetDelta();
#endif
	 //getEntity()->setRotation(m_camera->getDirection());
	 //m_weaponEntity->setRotation(m_camera->getDirection());

	glm::quat rot = glm::eulerAngleYX(glm::radians(-m_camera->m_yaw), glm::radians(m_camera->m_pitch));
	m_weaponEntity->setRotation(rot);

#if 0
	 glm::vec3 cameraDirection = CameraProxy::getInstance()->getDirection();
	 glm::vec3 pos = getEntity()->getPosition();
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
	 
	 getEntity()->setPosition(pos);
#endif
}

void ShockPlayerController::updateMovement(float dt)
{
	 glm::vec3 moveVector = glm::vec3(0.0f);
	 
	 InputManager* inputManager = InputManager::getInstance();
	 
	 bool isPlayerMove = (inputManager->getKey(KeyboardKeys::KEY_W)) ||
	 					(inputManager->getKey(KeyboardKeys::KEY_S)) ||
	 					(inputManager->getKey(KeyboardKeys::KEY_A)) ||
	 					(inputManager->getKey(KeyboardKeys::KEY_D));
	 
	 Camera* camera = CameraProxy::getInstance();
	 
#if 0
	 m_onTheGround = true;
	 if (isPlayerMove && m_onTheGround)
	 {
		 glm::vec3 dir = glm::vec3(0.0f);
		 if (inputManager->getKey(KeyboardKeys::KEY_W))
			 dir += camera->getDirection();
		 if (inputManager->getKey(KeyboardKeys::KEY_S))
			 dir -= camera->getDirection();
		 if (inputManager->getKey(KeyboardKeys::KEY_A))
			 dir -= glm::normalize(glm::cross(camera->getDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));
		 if (inputManager->getKey(KeyboardKeys::KEY_D))
			 dir += glm::normalize(glm::cross(camera->getDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));

		 // apply impulse to rigid body
		 //m_rigidBody->applyImpulse(dir);
		 //m_rigidBody->setDirection(glm::normalize(dir) * dt * 2.0f);
		 //m_rigidBody->setPositionForce(getEntity()->getPosition());
		 m_rigidBody->getCharacterController()->setVelocityForTimeInterval(
			 glmVectorToBt(glm::normalize(dir) * 2.0f),
			 2.0f);
	 }

	 if (!isPlayerMove)
		 m_rigidBody->getCharacterController()->setVelocityForTimeInterval(
			 glmVectorToBt(glm::vec3(0.0f)),
			 2.0f);
#else
	 m_onTheGround = true;
	 if (isPlayerMove && m_onTheGround)
	 {
		 glm::vec3 dir = glm::vec3(0.0f);
		 if (inputManager->getKey(KeyboardKeys::KEY_W))
			 dir += camera->getDirection();
		 if (inputManager->getKey(KeyboardKeys::KEY_S))
			 dir -= camera->getDirection();
		 if (inputManager->getKey(KeyboardKeys::KEY_A))
			 dir -= glm::normalize(glm::cross(camera->getDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));
		 if (inputManager->getKey(KeyboardKeys::KEY_D))
			 dir += glm::normalize(glm::cross(camera->getDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));

		 // apply impulse to rigid body
		 //m_rigidBody->applyImpulse(dir);
		 m_rigidBody->setDirection(glm::normalize(dir) * dt * 2.0f);
		 //m_rigidBody->setPositionForce(getEntity()->getPosition());
	 }

	 if (!isPlayerMove)
		 m_rigidBody->setDirection(glm::vec3(0.0f));

#endif

	 m_rigidBody->update(dt);

	 btTransform trans = m_rigidBody->getGhostObject()->getWorldTransform();
	 getEntity()->setPosition(btVectorToGlm(trans.getOrigin()));
}

void ShockPlayerController::debugUpdate(float dt)
{
	if (!m_rigidBody)
		return;

	char buf[256];
	
	// Ghost object stuff ...

	snprintf(buf, sizeof(buf), "--- Ghost Object ---");
	g_fontManager->drawSystemFontShadowed(buf, 0, 200, glm::vec4(1.0f));

	btTransform trans = m_rigidBody->getGhostObject()->getWorldTransform();
	
	glm::vec3 ghostObjectOrigin = btVectorToGlm(trans.getOrigin());
	snprintf(buf, sizeof(buf), "Origin: %.2f %.2f %.2f", 
		ghostObjectOrigin.x, 
		ghostObjectOrigin.y, 
		ghostObjectOrigin.z);

	g_fontManager->drawSystemFontShadowed(buf, 0, 220, glm::vec4(1.0f));

	btQuaternion ghostObjectRotation = trans.getRotation();
	snprintf(buf, sizeof(buf), "Basis: %.2f %.2f %.2f %.2f", 
		ghostObjectRotation.x(),
		ghostObjectRotation.y(),
		ghostObjectRotation.z(),
		ghostObjectRotation.w());

	g_fontManager->drawSystemFontShadowed(buf, 0, 240, glm::vec4(1.0f));

	snprintf(buf, sizeof(buf), "--- Character Controller ---");
	g_fontManager->drawSystemFontShadowed(buf, 0, 280, glm::vec4(1.0f));

	snprintf(buf, sizeof(buf), "Delta: %f", dt);
	g_fontManager->drawSystemFontShadowed(buf, 0, 300, glm::vec4(1.0f));

	btVector3 linearVelocity = m_rigidBody->getCharacterController()->getLinearVelocity();
	snprintf(buf, sizeof(buf), "Linear velocity: %.2f %.2f %.2f",
		ghostObjectRotation.x(),
		ghostObjectRotation.y(),
		ghostObjectRotation.z());

	g_fontManager->drawSystemFontShadowed(buf, 0, 320, glm::vec4(1.0f));

	btVector3 gravity = m_rigidBody->getCharacterController()->getGravity();
	snprintf(buf, sizeof(buf), "Gravity: %.2f %.2f %.2f",
		ghostObjectRotation.x(),
		ghostObjectRotation.y(),
		ghostObjectRotation.z());

	g_fontManager->drawSystemFontShadowed(buf, 0, 340, glm::vec4(1.0f));

	btScalar fallSpeed = m_rigidBody->getCharacterController()->getFallSpeed();
	snprintf(buf, sizeof(buf), "fallSpeed: %f", fallSpeed);
	g_fontManager->drawSystemFontShadowed(buf, 0, 360, glm::vec4(1.0f));

	bool onGround = m_rigidBody->getCharacterController()->onGround();
	snprintf(buf, sizeof(buf), "onGround: %s", onGround ? "true" : "false");
	g_fontManager->drawSystemFontShadowed(buf, 0, 380, glm::vec4(1.0f));
}

}