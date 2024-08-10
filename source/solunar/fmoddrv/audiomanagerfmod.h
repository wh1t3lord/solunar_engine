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

		virtual void init();
		virtual void shutdown();

		void update() override;

		virtual AudioSource* createSource(const std::string& filename);
		virtual void deleteSource(AudioSource* source);
	
		FMOD::System* getFMODSystem();

		FMOD::Studio::System* getStudioSystem() { return m_studioSystem; }

	private:
		FMODDrvSingletonStorer m_singletonStorer;

		FMOD::Studio::System* m_studioSystem;
	};
}

#endif // !AUDIOMANAGERFMOD_H
