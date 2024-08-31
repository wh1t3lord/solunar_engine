#include "miniaudiodrv_pch.h"
#include "miniaudiodrv/miniaudiosource.h"
#include "miniaudiodrv/miniaudiomanager.h"
#include "miniaudiodrv/miniaudiomusicmanager.h"

namespace solunar
{

MiniAudioMusicManager::MiniAudioMusicManager() :
	m_musicSource(nullptr)
{
}

MiniAudioMusicManager::~MiniAudioMusicManager()
{
}

void MiniAudioMusicManager::Init()
{
}

void MiniAudioMusicManager::Shutdown()
{
}

void MiniAudioMusicManager::Play(const std::string& filename, bool looped)
{
	if (m_musicSource)
		AudioManager::GetInstance()->DeleteSource(m_musicSource);
	m_musicSource = AudioManager::GetInstance()->CreateSource(filename);
	reinterpret_cast<MiniAudioSource*>(m_musicSource)->Play();
}

void MiniAudioMusicManager::Stop()
{
	if (m_musicSource)
	{
		m_musicSource->Stop();
		AudioManager::GetInstance()->DeleteSource(m_musicSource);
		m_musicSource = nullptr;
	}
}

bool MiniAudioMusicManager::IsPlaying()
{
	if (m_musicSource)
		return m_musicSource->IsPlaying();

	return false;
}

}