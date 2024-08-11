#ifndef GAMELOGIC_LEVELMANAGER_H
#define GAMELOGIC_LEVELMANAGER_H

namespace solunar
{
	
class LevelManagerComponent : public Component
{
	IMPLEMENT_OBJECT(LevelManagerComponent, Component);
public:
	LevelManagerComponent();
	~LevelManagerComponent();

	static void RegisterObject();
	
	void OnWorldSet(World* world) override;


private:

};

}

#endif // !GAMELOGIC_LEVELMANAGER_H
