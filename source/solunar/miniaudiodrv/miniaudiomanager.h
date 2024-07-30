#ifndef MINIAUDIOMANAGER_H
#define MINIAUDIOMANAGER_H

#include "engine/audio/audiomanager.h"

#include "miniaudiodrv/miniaudio_shared.h"
#include "miniaudiodrv/miniaudiomusicmanager.h"

namespace engine
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

	void init() override;
	void shutdown() override;

	void update() override;

	AudioSource* createSource(const std::string& filename) override;
	void deleteSource(AudioSource* source) override;

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
