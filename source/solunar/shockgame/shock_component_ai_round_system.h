// -----------------------------------------
//
//   author: wh1t3lord (https://github.com/wh1t3lord)
//	 description: Simple Round system for zombie waves
//
// -----------------------------------------

#ifndef SHOCK_AI_COMPONENT_ROUND_SYSTEM
#define SHOCK_AI_COMPONENT_ROUND_SYSTEM

namespace solunar
{
	class ShockAIRoundSystem : public LogicComponent
	{
		DECLARE_OBJECT(ShockAIRoundSystem);
		DECLARE_PROPERTY_REGISTER(ShockAIRoundSystem);
	public:
		ShockAIRoundSystem();
		~ShockAIRoundSystem();

		void Update(float dt) override;

		void LoadXML(tinyxml2::XMLElement& element) override;
		void SaveXML(tinyxml2::XMLElement& element) override;

	private:
		bool IsPlayerKilledEnoughZombies();
		void NextRound();
		void FinishRound();
		void CompleteGame();
		void CompleteLevel();
			
	private:
		unsigned char m_current_round;
		unsigned char m_max_rounds;
		int m_zombies_to_kill;
	};
}

#endif