#ifndef MINIAUDIOMUSICMANAGER_H
#define MINIAUDIOMUSICMANAGER_H

#include "engine/audio/musicmanager.h"

namespace engine
{

class AudioSource;

class MiniAudioMusicManager : public MusicManager
{
public:
	MiniAudioMusicManager();
	~MiniAudioMusicManager();

	void init();
	void shutdown();

	void play(const std::string& filename, bool looped = false);
	void stop();

	bool isPlaying();

private:
	AudioSource* m_musicSource;
};

}

#endif // !MINIAUDIOMUSICMANAGER_H
