#include "enginepch.h"
#include "engine/audio/audiomanager.h"

namespace engine
{
	AudioManager* AudioManager::ms_pInstance = nullptr;

	AudioManager::AudioManager()
	{

	}

	AudioManager::~AudioManager()
	{

	}

	void AudioManager::init()
	{

	}

	void AudioManager::shutdown()
	{

	}

	void AudioManager::update()
	{

	}

	AudioSource* AudioManager::createSource(const std::string& filename)
	{
		return nullptr;
	}

	void AudioManager::deleteSource(AudioSource* source)
	{

	}

	void AudioManager::setListenerPosition(const glm::vec3& position)
	{
	}

}
