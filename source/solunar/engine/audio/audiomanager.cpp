#include "enginepch.h"
#include "engine/audio/audiomanager.h"

namespace solunar
{
	AudioManager* AudioManager::ms_pInstance = nullptr;

	AudioManager::AudioManager()
	{

	}

	AudioManager::~AudioManager()
	{

	}

	void AudioManager::Init()
	{

	}

	void AudioManager::Shutdown()
	{

	}

	void AudioManager::Update()
	{

	}

	AudioSource* AudioManager::CreateSource(const std::string& filename)
	{
		return nullptr;
	}

	void AudioManager::DeleteSource(AudioSource* source)
	{

	}

}
