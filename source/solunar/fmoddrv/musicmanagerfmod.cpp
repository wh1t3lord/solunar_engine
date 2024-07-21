#include "fmoddrv_pch.h"
#include "fmoddrv/musicmanagerfmod.h"
#include "fmoddrv/audiomanagerfmod.h"
#include "fmoddrv/audiosourcefmod.h"

namespace engine
{
	MusicManagerFMOD::MusicManagerFMOD()
	{
		m_musicChannel = nullptr;
		m_musicSource = nullptr;
	}

	MusicManagerFMOD::~MusicManagerFMOD()
	{
	}

	void MusicManagerFMOD::init()
	{
		Assert(reinterpret_cast<AudioManagerFMOD*>(AudioManager::getInstance())->getFMODSystem());
		reinterpret_cast<AudioManagerFMOD*>(AudioManager::getInstance())->getFMODSystem()->createChannelGroup("Music channel group", &m_musicChannel);

		m_musicChannel->setVolume(0.3f);
	}

	void MusicManagerFMOD::shutdown()
	{
		if (m_musicSource && m_musicSource->isPlaying())
			m_musicSource->stop();

		if (m_musicSource)
			AudioManager::getInstance()->deleteSource(m_musicSource);

		m_musicSource = nullptr;

		m_musicChannel->release();
		m_musicChannel = nullptr;
	}

	void MusicManagerFMOD::play(const std::string& filename, bool looped /*= false*/)
	{
		if (m_musicSource)
			AudioManager::getInstance()->deleteSource(m_musicSource);

		m_musicSource = AudioManager::getInstance()->createSource(filename);


		reinterpret_cast<AudioSourceFMOD*>(m_musicSource)->play(m_musicChannel);

	}

	void MusicManagerFMOD::stop()
	{
		if (m_musicSource)
		{
			m_musicSource->stop();
			AudioManager::getInstance()->deleteSource(m_musicSource);
			m_musicSource = nullptr;
		}
	}

	bool MusicManagerFMOD::isPlaying()
	{
		if (m_musicSource)
			return (m_musicSource ? m_musicSource->isPlaying() : false);

		return false;
	}

}