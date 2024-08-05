#include "fmoddrv_pch.h"

#include "core/file/filesystem.h"

#include "engine/audio/musicmanager.h"

#include "fmoddrv/audiomanagerfmod.h"
#include "fmoddrv/audiosourcefmod.h"

#include <fmod_errors.h>

// #HACK
#if 0
#ifndef NDEBUG
#pragma comment(lib, "fmodstudioL_vc.lib")
#pragma comment(lib, "fmodL_vc.lib")
#else
#pragma comment(lib, "fmodstudio_vc.lib")
#pragma comment(lib, "fmod_vc.lib")
#endif // !NDEBUG
#endif

namespace engine
{

	static FMOD_RESULT FMOD_LoggerCallback(
		FMOD_DEBUG_FLAGS flags,
		const char* file,
		int line,
		const char* func,
		const char* message
	)
	{
		std::string messageString = message;
		messageString[messageString.size() - 1] = ' ';

		Core::msg("[fmod]: %s: %s", func, messageString.c_str());

		return FMOD_OK;
	}

	AudioManager* AudioManager::createInstance()
	{
		if (!ms_pInstance)
			ms_pInstance = mem_new<AudioManagerFMOD>();

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

	AudioManagerFMOD::AudioManagerFMOD() :
		m_studioSystem(nullptr)
	{
	}

	AudioManagerFMOD::~AudioManagerFMOD()
	{
	}

	void AudioManagerFMOD::init()
	{
#ifndef NDEBUG
		FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_LOG, FMOD_DEBUG_MODE_CALLBACK, &FMOD_LoggerCallback, nullptr);
#endif // !NDEBUG

		// Create the Studio System object.
		FMOD_RESULT result = FMOD::Studio::System::create(&m_studioSystem);
		if (result != FMOD_OK)
		{
			Core::error("Failed to create FMOD Studio. Error code:%s", FMOD_ErrorString(result));
		}

		// Initialize FMOD Studio, which will also initialize FMOD Core
		result = m_studioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
		if (result != FMOD_OK)
		{
			Core::error("Failed to initialize FMOD Studio. Error code:%s", FMOD_ErrorString(result));
		}

		Core::msg("[audio]: fmod initialized");

		// Initialize music manager.
		MusicManager::getInstance()->init();
	}

	void AudioManagerFMOD::shutdown()
	{
		MusicManager::getInstance()->shutdown();

		//m_fmodSystem->close();
		//m_fmodSystem->release();
		//m_fmodSystem = nullptr;
	}

	void AudioManagerFMOD::update()
	{
		//m_fmodSystem->update();
	}

	AudioSource* AudioManagerFMOD::createSource(const std::string& filename)
	{
		return (AudioSource*)mem_new<AudioSourceFMOD>(filename, getFMODSystem());
	}

	void AudioManagerFMOD::deleteSource(AudioSource* source)
	{
		if (source)
		{
			if (source->isPlaying())
				source->stop();

			mem_delete(source);
		}
	}

	FMOD::System* AudioManagerFMOD::getFMODSystem()
	{
		FMOD::System* coreSystem = nullptr;
		m_studioSystem->getCoreSystem(&coreSystem);

		Assert(coreSystem);

		return coreSystem;
	}
}
