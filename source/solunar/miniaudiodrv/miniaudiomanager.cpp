#include "miniaudiodrv_pch.h"
#include "miniaudiodrv/miniaudiomanager.h"
#include "miniaudiodrv/miniaudiosource.h"

namespace engine
{

// Singleton creation

AudioManager* AudioManager::createInstance()
{
	if (!ms_pInstance)
		ms_pInstance = mem_new<MiniAudioManager>();

	return ms_pInstance;
}

void AudioManager::destroyInstance()
{
	if (ms_pInstance)
	{
		mem_delete(ms_pInstance);
		ms_pInstance = nullptr;
	}
}

// MiniAudio Callback
void dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
	// In playback mode copy data to pOutput. In capture mode read data from pInput. In full-duplex mode, both
	// pOutput and pInput will be valid and you can move data from pInput into pOutput. Never process more than
	// frameCount frames.
}

// MiniAudioManager Implementation

MiniAudioManager::MiniAudioManager()
{
	memset(&m_device, 0, sizeof(m_device));
	memset(&m_resourceManagerConfig, 0, sizeof(m_resourceManagerConfig));
	memset(&m_resourceManager, 0, sizeof(m_resourceManager));
}

MiniAudioManager::~MiniAudioManager()
{
}

void MiniAudioManager::init()
{
	// Initialize device

	ma_device_config config = ma_device_config_init(ma_device_type_playback);
	config.playback.format = ma_format_f32;		// Set to ma_format_unknown to use the device's native format.
	config.playback.channels = 2;				// Set to 0 to use the device's native channel count.
	config.sampleRate = 48000;					// Set to 0 to use the device's native sample rate.
	config.dataCallback = dataCallback;			// This function will be called when miniaudio needs more data.
	config.pUserData = nullptr;					// Can be accessed from the device object (device.pUserData).

	if (ma_device_init(NULL, &config, &m_device) != MA_SUCCESS) {
		Core::error("MiniAudioManager::init: Failed to initialize the device.");
	}

	// Initialize resource manager
	
	m_resourceManagerConfig = ma_resource_manager_config_init();
	m_resourceManagerConfig.decodedFormat = m_device.playback.format;
	m_resourceManagerConfig.decodedChannels = m_device.playback.channels;
	m_resourceManagerConfig.decodedSampleRate = m_device.sampleRate;

	ma_result result = ma_resource_manager_init(&m_resourceManagerConfig, &m_resourceManager);
	if (result != MA_SUCCESS) {
		ma_device_uninit(&m_device);
		Core::error("MiniAudioManager::init: Failed to initialize the resource manager.");
	}

	// Start device
	ma_device_start(&m_device);

	Core::msg("MiniAudioManager::init: MiniAudio is initialized");
}

void MiniAudioManager::shutdown()
{
	// Shutdown resource manager
	ma_resource_manager_uninit(&m_resourceManager);

	// Shutdown device
	ma_device_uninit(&m_device);
}

void MiniAudioManager::update()
{
	// #TODO: update queue and remove sources from cache
	
	if (!m_deleteQueue.empty())
	{
		size_t deleteQueueSize = m_deleteQueue.size();
		m_deleteQueue.clear();

		Core::msg("MiniAudioManager::update: deleted %i audio sources", deleteQueueSize);
	}
}

AudioSource* MiniAudioManager::createSource(const std::string& filename)
{
	MiniAudioSource* miniAudioSource = mem_new<MiniAudioSource>(filename);
	m_audioSources.push_back(miniAudioSource);
	return miniAudioSource;
}

void MiniAudioManager::deleteSource(AudioSource* source)
{
	auto it = std::find(m_audioSources.begin(), m_audioSources.end(), source);
	Assert(it == m_audioSources.end());
	m_deleteQueue.push_back(source);
}

}
