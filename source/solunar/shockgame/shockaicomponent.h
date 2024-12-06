#ifndef SHOCKAICOMPONENT_H
#define SHOCKAICOMPONENT_H

#include "graphics/animatedmodel.h"
#include "graphics/mesh.h"

namespace solunar
{

enum ShockAIType
{
	ShockAIType_None,
	ShockAIType_Camera,
	ShockAIType_Zombie,
};

struct ShockAIZombieData
{
	int m_idleAnimation;
	int m_walkAnimation;
	int m_attackAnimation;
	int m_dieAnimation;

	ShockAIZombieData() :
		m_idleAnimation(-1),
		m_walkAnimation(-1),
		m_attackAnimation(-1),
		m_dieAnimation(-1)
	{
	}
};

class ShockAIComponent : public LogicComponent
{
	DECLARE_OBJECT(ShockAIComponent);
	DECLARE_PROPERTY_REGISTER(ShockAIComponent);
public:
	ShockAIComponent();
	~ShockAIComponent();

	void OnInit() override;

	void Update(float dt) override;
	void updateAICamera(float dt);
	void updateFire(float dt);

	void UpdateZombie(float dt);
	void UpdateZombie_AnimationController(float dt);

	void LoadXML(tinyxml2::XMLElement& element) override;
	void SaveXML(tinyxml2::XMLElement& element) override;

private:
	ShockAIZombieData m_zombieData;
	AnimatedMeshComponent* m_animatedComponent;
	ShockAIType m_aiType;
	bool m_fire;
	bool m_disable;
};

ShockAIType GetShockAITypeFromString(const std::string& name);
std::string ShockAITypeToString(ShockAIType type);

}

#endif // !SHOCKAICOMPONENT_H
