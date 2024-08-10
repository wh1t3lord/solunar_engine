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

		void init();
		void shutdown();

		void play(const std::string& filename, bool looped = false);
		void stop();

		bool isPlaying();

	private:
		FMOD::ChannelGroup* m_musicChannel;
		AudioSource* m_musicSource;
	};
}

#endif // !MUSICMANAGERFMOD_H
