#ifndef MINIAUDIOMUSICMANAGER_H
#define MINIAUDIOMUSICMANAGER_H

#include "engine/audio/musicmanager.h"

namespace solunar
{

class AudioSource;

class MiniAudioMusicManager : public MusicManager
{
public:
	MiniAudioMusicManager();
	~MiniAudioMusicManager();

	void Init();
	void Shutdown();

	void Play(const std::string& filename, bool looped = false);
	void Stop();

	bool IsPlaying();

private:
	AudioSource* m_musicSource;
};

}

#endif // !MINIAUDIOMUSICMANAGER_H
