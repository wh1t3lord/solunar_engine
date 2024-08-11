#ifndef MUSICMANAGERFMOD_H
#define MUSICMANAGERFMOD_H

#include "engine/audio/musicmanager.h"

namespace solunar
{
	class AudioSource;

	class MusicManagerFMOD : public MusicManager
	{
	public:
		MusicManagerFMOD();
		~MusicManagerFMOD();

		void Init();
		void Shutdown();

		void Play(const std::string& filename, bool looped = false);
		void Stop();

		bool IsPlaying();

	private:
		FMOD::ChannelGroup* m_musicChannel;
		AudioSource* m_musicSource;
	};
}

#endif // !MUSICMANAGERFMOD_H
