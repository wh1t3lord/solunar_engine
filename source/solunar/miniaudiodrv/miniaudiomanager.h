#ifndef MINIAUDIOMANAGER_H
#define MINIAUDIOMANAGER_H

#include "engine/audio/audiomanager.h"

#include "miniaudiodrv/miniaudio_shared.h"
#include "miniaudiodrv/miniaudiomusicmanager.h"

namespace solunar
{

struct MiniAudioSingletonStorer
{
	MiniAudioMusicManager m_musicManager;
};

class MiniAudioManager : public AudioManager
{
public:
	MiniAudioManager();
	~MiniAudioManager();

	void Init() override;
	void Shutdown() override;

	void Update() override;

	AudioSource* CreateSource(const std::string& filename) override;
	void DeleteSource(AudioSource* source) override;

private:
	MiniAudioSingletonStorer m_singletonStorer;

	// MiniAudio Device
	ma_device m_device;

	// MiniAudio Resource Manager
	ma_resource_manager_config m_resourceManagerConfig;
	ma_resource_manager m_resourceManager;

	// Audio source storage
	std::vector<AudioSource*> m_audioSources;

	// Audio source delete queue
	std::vector<AudioSource*> m_deleteQueue;

};

}

#endif // !MINIAUDIOMANAGER_H
