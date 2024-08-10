#include "fmoddrv_pch.h"

#include "core/file/filesystem.h"
#include "core/file/contentdevice.h"
#include "core/file/contentmanager.h"

#include "fmoddrv/audiosourcefmod.h"

namespace engine
{
	AudioSourceFMOD::AudioSourceFMOD(const std::string& filename, FMOD::System* system)
	{
		m_soundChannel = nullptr;

		Assert(system);
		m_system = system;

		DataStreamPtr file = g_contentManager->openStream(filename);

		file->seek(Seek_End, 0);
		size_t size = file->tell();
		file->seek(Seek_Begin, 0);

		char* data = (char*)malloc(size * sizeof(char));
		file->read(data, size);

		FMOD_CREATESOUNDEXINFO exinfo;
		memset(&exinfo, 0, sizeof(exinfo));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.length = size;

		FMOD_RESULT result = m_system->createSound(data, FMOD_OPENMEMORY, &exinfo, &m_sound);
		if (result != FMOD_OK)
		{
			Core::error("[audio]: failed to create sound! FMOD ERROR: %s", getStringFromFMODResult(result).c_str());
		}

		free(data);
	}

	AudioSourceFMOD::~AudioSourceFMOD()
	{

	}

	bool AudioSourceFMOD::isPlaying()
	{
		bool playing = false;
		FMOD_RESULT result = m_soundChannel->isPlaying(&playing);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		{
			Core::error("[audio]: AudioSourceFMOD::isPlaying: %s", getStringFromFMODResult(result).c_str());
		}
		return playing;
	}

	void AudioSourceFMOD::play()
	{
		Assert(m_system);
		Assert(m_sound);

		FMOD_RESULT result = FMOD_OK;

		if (!m_soundChannel)
		{
			result = m_system->playSound(m_sound, 0, false, &m_soundChannel);
			if (result != FMOD_OK)
			{
				Core::error("[audio]: failed to play sound! [audio]: FMOD ERROR: %s", 
				getStringFromFMODResult(result).c_str());
			}
		}

		if (m_soundChannel)
		{
			if (isPlaying())
				stop();

			result = m_system->playSound(m_sound, 0, false, &m_soundChannel);
			if (result != FMOD_OK)
			{
				Core::error("[audio]: failed to play sound![audio]: FMOD ERROR: %s", 
				getStringFromFMODResult(result).c_str());
			}
		}
	}

	void AudioSourceFMOD::play(FMOD::ChannelGroup* channelGroup, bool looped /*= false*/)
	{
		Assert(channelGroup);
		Assert(m_system);
		Assert(m_sound);

		FMOD_RESULT result = FMOD_OK;

		if (!m_soundChannel)
		{
			result = m_system->playSound(m_sound, channelGroup, false, &m_soundChannel);
			if (result != FMOD_OK)
			{
				Core::error("[audio]: failed to play sound! [audio]: FMOD ERROR: %s", 
				getStringFromFMODResult(result).c_str());
			}

			m_soundChannel->setLoopCount(-1);
		}

		if (m_soundChannel)
		{
			if (isPlaying())
				stop();

			result = m_system->playSound(m_sound, channelGroup, false, &m_soundChannel);
			if (result != FMOD_OK)
			{
				Core::error("[audio]: failed to play sound![audio]: FMOD ERROR: %s", 
				getStringFromFMODResult(result).c_str());
			}
		}
	}

	void AudioSourceFMOD::stop()
	{
		if (!m_soundChannel)
		{
			Core::error("[audio]: trying to stop sound when his sounds channel is not created!");
		}

		m_soundChannel->stop();
	}
}