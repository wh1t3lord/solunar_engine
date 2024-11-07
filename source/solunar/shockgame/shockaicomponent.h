#ifndef SHOCKAICOMPONENT_H
#define SHOCKAICOMPONENT_H

namespace solunar
{

enum ShockAIType
{
	ShockAIType_None,
	ShockAIType_Camera,
	ShockAIType_Zombie,
};

class ShockAIComponent : public LogicComponent
{
	DECLARE_OBJECT(ShockAIComponent);
	DECLARE_PROPERTY_REGISTER(ShockAIComponent);
public:
	ShockAIComponent();
	~ShockAIComponent();

	void Update(float dt) override;
	void updateAICamera(float dt);
	void updateFire(float dt);

	void UpdateZombie(float dt);

	void LoadXML(tinyxml2::XMLElement& element) override;
	void SaveXML(tinyxml2::XMLElement& element) override;

private:
	ShockAIType m_aiType;
	bool m_fire;
	bool m_disable;
};

ShockAIType GetShockAITypeFromString(const std::string& name);
std::string ShockAITypeToString(ShockAIType type);

}

#endif // !SHOCKAICOMPONENT_H
