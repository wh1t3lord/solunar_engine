#include "fmoddrv_pch.h"
#include "fmoddrv/musicmanagerfmod.h"
#include "fmoddrv/audiomanagerfmod.h"
#include "fmoddrv/audiosourcefmod.h"

namespace solunar
{
	MusicManagerFMOD::MusicManagerFMOD()
	{
		m_musicChannel = nullptr;
		m_musicSource = nullptr;
	}

	MusicManagerFMOD::~MusicManagerFMOD()
	{
	}

	void MusicManagerFMOD::Init()
	{
		Assert(reinterpret_cast<AudioManagerFMOD*>(AudioManager::GetInstance())->GetFMODSystem());
		reinterpret_cast<AudioManagerFMOD*>(AudioManager::GetInstance())->GetFMODSystem()->createChannelGroup("Music channel group", &m_musicChannel);

		m_musicChannel->setVolume(0.3f);
	}

	void MusicManagerFMOD::Shutdown()
	{
		if (m_musicSource && m_musicSource->IsPlaying())
			m_musicSource->Stop();

		if (m_musicSource)
			AudioManager::GetInstance()->DeleteSource(m_musicSource);

		m_musicSource = nullptr;

		m_musicChannel->release();
		m_musicChannel = nullptr;
	}

	void MusicManagerFMOD::Play(const std::string& filename, bool looped /*= false*/)
	{
		if (m_musicSource)
			AudioManager::GetInstance()->DeleteSource(m_musicSource);

		m_musicSource = AudioManager::GetInstance()->CreateSource(filename);


		reinterpret_cast<AudioSourceFMOD*>(m_musicSource)->Play(m_musicChannel);

	}

	void MusicManagerFMOD::Stop()
	{
		if (m_musicSource)
		{
			m_musicSource->Stop();
			AudioManager::GetInstance()->DeleteSource(m_musicSource);
			m_musicSource = nullptr;
		}
	}

	bool MusicManagerFMOD::IsPlaying()
	{
		if (m_musicSource)
			return m_musicSource->IsPlaying();

		return false;
	}

}