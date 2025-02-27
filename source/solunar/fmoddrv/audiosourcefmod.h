#ifndef AUDIOSOURCEFMOD_H
#define AUDIOSOURCEFMOD_H

#include "engine/audio/audiosource.h"

namespace solunar
{
	class AudioSourceFMOD : public AudioSource
	{
	public:
		AudioSourceFMOD(const std::string& filename, FMOD::System* system);
		~AudioSourceFMOD();

		bool IsPlaying() override;

		void Play() override;
		void Play(FMOD::ChannelGroup* channelGroup, bool looped = false);
		void Stop() override;

		FMOD::Sound* getSound() { return m_sound; }
		FMOD::Channel* getSoundChannel() { return m_soundChannel; }

	private:
		FMOD::System* m_system;
		FMOD::Sound* m_sound;
		FMOD::Channel* m_soundChannel;
	};
}

#endif // !AUDIOSOURCEFMOD_H
