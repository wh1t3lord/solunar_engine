#ifndef AUDIOMANAGERFMOD_H
#define AUDIOMANAGERFMOD_H

#include "engine/audio/audiomanager.h"

#include "fmoddrv/musicmanagerfmod.h"

namespace solunar
{
	struct FMODDrvSingletonStorer
	{
		MusicManagerFMOD m_musicManager;
	};

	class AudioManagerFMOD : public AudioManager
	{
	public:
		AudioManagerFMOD();
		~AudioManagerFMOD();

		virtual void Init();
		virtual void Shutdown();

		void Update() override;

		virtual AudioSource* CreateSource(const std::string& filename);
		virtual void DeleteSource(AudioSource* source);
	
		void setListenerPosition(const glm::vec3& position);
	
		FMOD::System* GetFMODSystem();
		FMOD::Studio::System* getStudioSystem() { return m_studioSystem; }

	private:
		FMODDrvSingletonStorer m_singletonStorer;

		FMOD::Studio::System* m_studioSystem;
	};
}

#endif // !AUDIOMANAGERFMOD_H
