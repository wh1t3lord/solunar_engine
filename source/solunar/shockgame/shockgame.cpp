#include "shockgamepch.h"

#include "shockgame/shockgame.h"
#include "shockgame/shockeventlistener.h"
#include "shockgame/shocksignalmanager.h"
#include "shockgame/shockplayercontroller.h"
#include "shockgame/shockgameui.h"

// Base game includes
#include "game/gamelogic/camera/cameramanager.h"
#include "game/gamelogic/mainmenu/mainmenuworldcomponent.h"
#include "game/gamelogic/player/playercontrollercomponent.h"
#include "game/gamelogic/player/weaponchaincomponent.h"
#include "game/gamelogic/saveload/saveloadmanager.h"
#include "game/gamelogic/weapons/weaponcomponent.h"
#include "game/gamelogic/weapons/weaponautocomponent.h"
#include "game/gamelogic/weapons/weaponpistolcomponent.h"
#include "game/gamelogic/world/levelmanager.h"

// Engine
#include "engine/camera.h"
#include "engine/engine.h"

// Graphics
#include "graphics/fontmanager.h"
#include "graphics/imguimanager.h"
#include "graphics/debugrenderer.h"

#undef min
#undef max

#include <numeric>

// Graphics Objects
#include "graphics/mesh.h"
#include "graphics/animatedmodel.h"

#include <array>

#include "core/object/propertymanager.h"

namespace engine
{

// implemetation for IGameInterface

static ShockGameInterface s_shockGameInterface;
IGameInterface* g_gameInterface = (IGameInterface*)&s_shockGameInterface;

ShockGameInterface* getShockGameInterface()
{
	return &s_shockGameInterface;
}

class TestRotatorComponent : public LogicComponent
{
	ImplementObject(TestRotatorComponent, LogicComponent);

public:
	TestRotatorComponent() : m_YAxis(0.0f)
	{}

	~TestRotatorComponent()
	{}

	static void registerObject()
	{
		g_typeManager->registerObject<TestRotatorComponent>();
	}

	void update(float dt) override
	{
		m_YAxis += dt * 200.0f;
		getEntity()->quaternionRotate(glm::vec3(0.f, 1.f, 0.f), m_YAxis);
	}

private:
	float m_YAxis;
};

class TestPositionUpdaterComponent : public LogicComponent
{
	ImplementObject(TestPositionUpdaterComponent, LogicComponent);

public:
	TestPositionUpdaterComponent()
	{}

	~TestPositionUpdaterComponent()
	{}

	static void registerObject()
	{
		g_typeManager->registerObject<TestPositionUpdaterComponent>();
	}

	void update(float dt) override
	{
		const float kSpeed = 2.0f;

		glm::vec3 position = getEntity()->getPosition();
		position.y += kSpeed * dt;
		getEntity()->setPosition(position);
	}

};

class TestAnimationComponent : public LogicComponent
{
	ImplementObject(TestAnimationComponent, LogicComponent);
public:
	TestAnimationComponent();
	~TestAnimationComponent();

	void update(float dt) override;

private:
	int m_animationIndex = -1;
};

TestAnimationComponent::TestAnimationComponent()
{
}

TestAnimationComponent::~TestAnimationComponent()
{
}

void TestAnimationComponent::update(float dt)
{
	AnimatedMeshComponent* animatedMeshComponent = getEntity()->getComponent<AnimatedMeshComponent>();
	std::shared_ptr<ModelBase> modelBase = animatedMeshComponent->lockModel();
	AnimatedModel* animatedModel = dynamicCast<AnimatedModel>(modelBase.get());
	if (animatedModel) {
		if (m_animationIndex == -1) {
			m_animationIndex = animatedModel->getAnimationByName("idle");
			animatedModel->setPlayAnimation(m_animationIndex, true);
		}
	}

	animatedModel->testPlay(dt);
}

class ViewmodelAnimationController : public LogicComponent
{
	ImplementObject(ViewmodelAnimationController, LogicComponent);
public:
	ViewmodelAnimationController();
	~ViewmodelAnimationController();

	void update(float dt) override;

private:
	int m_animationIndex = -1;
};

ViewmodelAnimationController::ViewmodelAnimationController()
{
}

ViewmodelAnimationController::~ViewmodelAnimationController()
{
}

void ViewmodelAnimationController::update(float dt)
{
	AnimatedMeshComponent* animatedMeshComponent = getEntity()->getComponent<AnimatedMeshComponent>();
	std::shared_ptr<ModelBase> modelBase = animatedMeshComponent->lockModel();
	AnimatedModel* animatedModel = dynamicCast<AnimatedModel>(modelBase.get());
	if (animatedModel) {
		if (m_animationIndex == -1) {
			m_animationIndex = animatedModel->getAnimationByName("Armature|Armature|Hide");
			animatedModel->setPlayAnimation(m_animationIndex, true);
			animatedModel->pauseAnimationPlay();
		}
	}

	animatedModel->testPlay(dt);
}

enum ShockProjectileType
{
	ShockProjectileType_Default,
	ShockProjectileType_Camera,
	ShockProjectileType_Pistol,
	ShockProjectileType_AssaultRifle,
	ShockProjectileType_Shotgun,
	ShockProjectileType_Grenade,
};

class ShockProjectileComponent : public LogicComponent
{
	ImplementObject(ShockProjectileComponent, LogicComponent);
public:
	ShockProjectileComponent();
	~ShockProjectileComponent();

	// #TODO: Make it key-values based and method of base class!
	void onSpawn(const glm::vec3& velocity, const glm::vec3& direction,
		const std::string& visualFilename);

	void onEntitySet(Entity* entity) override;

	void update(float dt) override;

private:
	glm::vec3 m_direction;
};

ShockProjectileComponent::ShockProjectileComponent() :
	m_direction(0.0f)
{
}

ShockProjectileComponent::~ShockProjectileComponent()
{
}

void ShockProjectileComponent::onSpawn(const glm::vec3& velocity, const glm::vec3& direction,
	const std::string& visualFilename)
{
#if 0
	// create rigid body
	BoxShapeComponent* boxShape = getEntity()->createComponent<BoxShapeComponent>();
	boxShape->createShape(glm::vec3(0.1f));

	RigidBodyComponent* rigidBody = getEntity()->createComponent<RigidBodyComponent>();
	rigidBody->attachShape(boxShape);
	rigidBody->setMass(0.02f);
	rigidBody->setLinearVelocity(velocity);
#endif

	m_direction = velocity;
}

void ShockProjectileComponent::onEntitySet(Entity* entity)
{
	Component::onEntitySet(entity);

	Assert2(!entity->getComponent<RigidBodyComponent>(), "Projectile cannot' have already attached rigid body!");
}

void ShockProjectileComponent::update(float dt)
{
	g_debugRender.drawBoundingBox(getEntity()->getBoundingBox(), glm::vec3(1.0f, 0.0, 0.0f));

	const float kSpeed = 1.2f;

	glm::vec3 position = getEntity()->getPosition();
	position = glm::lerp(position, m_direction, kSpeed * dt);
	getEntity()->setPosition(position);

	// find player
	std::vector<Entity*> players = getWorld()->getEntityManager().getEntitiesWithComponent<ShockPlayerController>();
	Entity* player = players[0];

	if (getEntity()->getBoundingBox().contains(player->getPosition())) // we shot!
	{
		ShockPlayerController* playerController = player->getComponent<ShockPlayerController>();
		playerController->doHit(25.0f);
	}
}

class ShockProjectilePool : public Singleton<ShockProjectilePool>
{
	static ShockProjectilePool ms_instance;
public:
	ShockProjectilePool();
	~ShockProjectilePool();

	/**
		/brief Return any free Entity from pool.
		/param projectileType	Specific projectile type (ShockProjectileType_Default have very small velocity value)
		/param position			Position of projectile starting point
		/param direction		Normalized direction for projectile
		/param velocity			Velocity of projectile(will be ignored by any projectile execpt ShockProjectileType_Default and ShockProjectileType_Grenade)
	*/
	Entity* createProjectile(ShockProjectileType projectileType,
		const glm::vec3& position,
		const glm::vec3& direction,
		const glm::vec3& velocity = glm::vec3(1.0f));

private:
	std::array<Entity*, 256> m_pool;
	int m_poolCount;
};

ShockProjectilePool ShockProjectilePool::ms_instance;

ShockProjectilePool::ShockProjectilePool()
{
	for (int i = 0; i < m_pool.size(); i++)
		m_pool[i] = nullptr;
}

ShockProjectilePool::~ShockProjectilePool()
{
}

Entity* ShockProjectilePool::createProjectile(ShockProjectileType projectileType, const glm::vec3& position, const glm::vec3& direction, const glm::vec3& velocity)
{
	Assert2(m_poolCount > 256, "Reached maximum of the pool");

	Entity* entity = m_pool[m_poolCount++];
	entity->setPosition(position);

	return entity;
}

enum ShockAIType
{
	ShockAIType_None,
	ShockAIType_Camera,
};

ShockAIType getShockAITypeFromString(const std::string& name)
{
	if (name == "ShockAIType_None")
		return ShockAIType_None;
	if (name == "ShockAIType_Camera")
		return ShockAIType_Camera;

	Assert2(0, "Unknowed AI Type");
	return ShockAIType_None;
}

std::string shockAITypeToString(ShockAIType type)
{
	switch (type)
	{
	case ShockAIType_None:
		return "ShockAIType_None";
	case ShockAIType_Camera:
		return "ShockAIType_Camera";
	}

	Assert2(0, "Unknowed AI Type");
	return std::string();
}

class ShockAIComponent : public LogicComponent
{
	ImplementObject(ShockAIComponent, LogicComponent);
public:
	ShockAIComponent();
	~ShockAIComponent();

	void update(float dt) override;
	void updateAICamera(float dt);
	void updateFire(float dt);

	void loadXML(tinyxml2::XMLElement& element) override;
	void saveXML(tinyxml2::XMLElement& element) override;

private:
	ShockAIType m_aiType;
	bool m_fire;
	bool m_disable;
};

ShockAIComponent::ShockAIComponent() :
	m_aiType(ShockAIType_None),
	m_fire(false),
	m_disable(false)
{
}

ShockAIComponent::~ShockAIComponent()
{
}

void ShockAIComponent::update(float dt)
{
	if (m_aiType == ShockAIType_Camera)
	{
		updateAICamera(dt);
	}
}

void ShockAIComponent::updateAICamera(float dt)
{
	// find player
	std::vector<Entity*> players = getWorld()->getEntityManager().getEntitiesWithComponent<ShockPlayerController>();

	Entity* player = players[0];

	// check distance to the camera
	glm::vec3 distance = player->getPosition() - getEntity()->getPosition();

	char aiDebugText[64];
	snprintf(aiDebugText, sizeof(aiDebugText), "distance %.2f %.2f %.2f", 
		distance.x, distance.y, distance.z);

	g_fontManager->drawSystemFontShadowed(aiDebugText, 0, 100, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	snprintf(aiDebugText, sizeof(aiDebugText), "length %.2f", glm::length(distance));
	g_fontManager->drawSystemFontShadowed(aiDebugText, 0, 120, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));


	if (glm::length(distance) < 8.0f)
	{
		m_fire = true;
	}

	if (m_fire && !m_disable)
	{
		Entity* projectile = Engine::ms_world->createEntity();
		projectile->setPosition(getEntity()->getPosition());

		ShockProjectileComponent* projectileComponent = projectile->createComponent<ShockProjectileComponent>();
		//projectileComponent->onSpawn(glm::normalize(player->getPosition()), glm::vec3(1.0f), "test");
		projectileComponent->onSpawn(player->getPosition(), glm::vec3(1.0f), "test");

		m_fire = false;
		m_disable = true;
	}

	// rotate camera torwads to player
	glm::quat rotation = glm::quatLookAt(glm::normalize(player->getPosition() - getEntity()->getPosition()), glm::vec3(0.0f, 1.0f, 0.0f));
	getEntity()->setRotation(glm::slerp(getEntity()->getRotation(), rotation, dt * 0.8f));
}

void ShockAIComponent::updateFire(float dt)
{

}

void ShockAIComponent::loadXML(tinyxml2::XMLElement& element)
{
	const tinyxml2::XMLElement* aitypeElement = element.FirstChildElement("AIType");

	const char* aitypeString = nullptr;
	aitypeElement->QueryAttribute("value", &aitypeString);

	if (aitypeString)
		m_aiType = getShockAITypeFromString(aitypeString);
}

void ShockAIComponent::saveXML(tinyxml2::XMLElement& element)
{
	std::string aitypeString = shockAITypeToString(m_aiType);

	tinyxml2::XMLElement* aitype = element.InsertNewChildElement("AIType");
	aitype->SetAttribute("value", aitypeString.c_str());
}

//void registerGameClasses()
//{
//	MainMenuWorldComponent),
//	PlayerControllerComponent),
//	WeaponComponent),
//	WeaponAutoComponent),
//	WeaponPistolComponent),
//	WeaponChainComponent),
//	LevelManagerComponent),
//
//	TestRotatorComponent),
//}

//void registerShockClasses()
//{
//	ShockSignal::registerObject();
//	ShockPlayerController::registerObject();
//}

class PropertyTestObject : public Object
{
	ImplementObject(PropertyTestObject, Object);
public:
	PropertyTestObject();
	~PropertyTestObject();

	static void registerProperties();

private:
	BoundingBox m_boundingBox;
	glm::vec2 m_vec2;
	glm::vec3 m_vec3;
	glm::vec4 m_vec4;
	glm::quat m_quat;
	glm::mat4 m_matrix;
	std::string m_string;
};

PropertyTestObject::PropertyTestObject()
{
	m_boundingBox.setIdentity();
	m_vec2 = glm::vec2(1.0f);
	m_vec3 = glm::vec3(1.0f);
	m_vec4 = glm::vec4(1.0f);
	m_quat = glm::identity<glm::quat>();
	m_matrix = glm::identity<glm::mat4>();
	m_string = "Test String";
}

PropertyTestObject::~PropertyTestObject()
{
}

#define REGISTER_PROPERTY(className, propertyType, propertyName) \
	PropertyManager::getInstance()->registerProperty(className::getStaticTypeInfo(), mem_new<propertyType>(#propertyName, offsetof(className, propertyName)));

void PropertyTestObject::registerProperties()
{
	REGISTER_PROPERTY(PropertyTestObject, PropertyBoundingBox, m_boundingBox);
	REGISTER_PROPERTY(PropertyTestObject, PropertyVector2, m_vec2);
	REGISTER_PROPERTY(PropertyTestObject, PropertyVector3, m_vec3);
	REGISTER_PROPERTY(PropertyTestObject, PropertyVector4, m_vec4);
	REGISTER_PROPERTY(PropertyTestObject, PropertyQuaternion, m_quat);
	REGISTER_PROPERTY(PropertyTestObject, PropertyMatrix4, m_matrix);
	REGISTER_PROPERTY(PropertyTestObject, PropertyString, m_string);
	//PropertyManager::getInstance()->registerProperty(typeInfo, mem_new<PropertyBoundingBox>("m_boundingBox", offsetof(PropertyTestObject, m_boundingBox)));
	//PropertyManager::getInstance()->registerProperty(typeInfo, mem_new<PropertyString>("m_string", offsetof(PropertyTestObject, m_string)));
}

void testRegisteringProperties()
{
	TypeManager::getInstance()->registerObject<PropertyTestObject>();

	PropertyTestObject::registerProperties();

	// test accessing
	PropertyTestObject* propertyTestObject = mem_new<PropertyTestObject>();

	IProperty* bboxProperty = PropertyManager::getInstance()->findProperty(PropertyTestObject::getStaticTypeInfo(), "m_boundingBox");
	IProperty* stringProperty = PropertyManager::getInstance()->findProperty(PropertyTestObject::getStaticTypeInfo(), "m_string");

	BoundingBox boundingBox;
	PropertyGetValue(propertyTestObject, bboxProperty, boundingBox);

	std::string stringValue;
	PropertyGetValue(propertyTestObject, stringProperty, stringValue);
	
	mem_delete(propertyTestObject);
}

// More beautiful way to register classes
void registerGameClasses()
{
	const TypeInfo* gameClasses[] = 
	{
		ObjectGetTypeInfo(MainMenuWorldComponent),
		ObjectGetTypeInfo(PlayerControllerComponent),
		ObjectGetTypeInfo(WeaponComponent),
		ObjectGetTypeInfo(WeaponAutoComponent),
		ObjectGetTypeInfo(WeaponPistolComponent),
		ObjectGetTypeInfo(WeaponChainComponent),
		ObjectGetTypeInfo(LevelManagerComponent),
		ObjectGetTypeInfo(TestRotatorComponent),
		ObjectGetTypeInfo(TestPositionUpdaterComponent),
		ObjectGetTypeInfo(TestAnimationComponent),
		ObjectGetTypeInfo(ViewmodelAnimationController),
	};

	for (int i = 0; i < sizeof(gameClasses) / sizeof(gameClasses[0]); i++)
		TypeManager::getInstance()->registerType(gameClasses[i]);
}

void registerShockClasses()
{
	const TypeInfo* shockClasses[] =
	{
		ObjectGetTypeInfo(ShockSignal),
		ObjectGetTypeInfo(ShockPlayerController),
		ObjectGetTypeInfo(ShockAIComponent),
		ObjectGetTypeInfo(ShockProjectileComponent),
		ObjectGetTypeInfo(ShockGameMainMenuComponent),
	};

	for (int i = 0; i < sizeof(shockClasses) / sizeof(shockClasses[0]); i++)
		TypeManager::getInstance()->registerType(shockClasses[i]);
}

void exportClassesForEditor()
{
	tinyxml2::XMLDocument doc;

	tinyxml2::XMLNode* pRoot = doc.NewElement("Classes");
	doc.InsertFirstChild(pRoot);

	std::vector<const TypeInfo*> registeredTypes;
	TypeManager::getInstance()->getRegisteredTypes(registeredTypes);

	for (auto it : registeredTypes)
	{
		tinyxml2::XMLElement* component = doc.NewElement("Class");
		component->SetAttribute("classname", it->getClassName());
		pRoot->InsertFirstChild(component);
	}

	doc.SaveFile("editor_classes.xml");
}

ShockGameInterface::ShockGameInterface()
{
}

ShockGameInterface::~ShockGameInterface()
{
}

void ShockGameInterface::initialize()
{
	Core::msg("Initializing game");

	// register game objects
	registerGameClasses();

	// register shock objects
	registerShockClasses();

	testRegisteringProperties();

	exportClassesForEditor();

	// add event listener
//	g_eventManager.addEventListener(&g_shockEventListener);

	// Create signal manager
	ShockSignalManager::createInstance();
}

void ShockGameInterface::shutdown()
{
	// Destroy signal manager
	ShockSignalManager::destroyInstance();

	// g_eventManager.removeEventListener(&g_shockEventListener);
}

void shockGamePlayerDebug(bool* open)
{
	if (ImGui::Begin("Shock Player Debug", open))
	{
		World* world = Engine::ms_world;
		if (world)
		{
			std::vector<Entity*> players = world->getEntityManager().getEntitiesWithComponent<ShockPlayerController>();
			if (!players.empty())
			{
				Entity* player = players[0];
				ShockPlayerController* playerController = player->getComponent<ShockPlayerController>();
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