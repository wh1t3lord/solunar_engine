#include "shockgamepch.h"
#include "shockgame/shockplayercontroller.h"

#include "engine/inputmanager.h"

#include "graphics/ifontmanager.h"

namespace engine
{

ShockPlayerController::ShockPlayerController() :
	m_camera(nullptr),
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

	m_rigidBody = getEntity()->createComponent<RigidBodyComponent>();

	m_shape = getEntity()->createComponent<BoxShapeComponent>();
	m_shape->createShape(glm::vec3(0.2f, 0.8f, 0.2f));

	m_rigidBody->attachShape(m_shape);

	activateCamera();
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

	View* view = CameraProxy::getInstance()->getView();
	
	char healthText[64];
	snprintf(healthText, sizeof(healthText), "Health: %.0f", m_playerStats.m_health);
	
	char enduranceText[64];
	snprintf(enduranceText, sizeof(enduranceText), "Endurance: %.0f", m_playerStats.m_endurance);

	g_fontManager->drawSystemFont(healthText, 25, view->m_height - 50, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));
	g_fontManager->drawSystemFont(enduranceText, 25, view->m_height - 25, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));

	// update camera look
	updateCamera(dt);

	// update player movement
	updateMovement(dt);

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
}

void ShockPlayerController::updateCamera(float dt)
{
	 InputManager* input = InputManager::getInstance();
	 glm::vec2 mousePos = input->getCursorPos();
	 glm::vec2 deltaMousePos = input->getDeltaCursorPos();
	 
	 if (input->getCursorPosCallback())
	 {
	 	m_camera->updateFromMousePosition(deltaMousePos);
	 	getEntity()->setRotation(m_camera->getDirection());
	 	input->setCursorPosCallback(false);
	 }
	 
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
	 	m_rigidBody->applyImpulse(dir);
	 }
}

}