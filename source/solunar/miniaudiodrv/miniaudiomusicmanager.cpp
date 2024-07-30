#include "miniaudiodrv_pch.h"
#include "miniaudiodrv/miniaudiosource.h"
#include "miniaudiodrv/miniaudiomanager.h"
#include "miniaudiodrv/miniaudiomusicmanager.h"

namespace engine
{

MiniAudioMusicManager::MiniAudioMusicManager() :
	m_musicSource(nullptr)
{
}

MiniAudioMusicManager::~MiniAudioMusicManager()
{
}

void MiniAudioMusicManager::init()
{
}

void MiniAudioMusicManager::shutdown()
{
}

void MiniAudioMusicManager::play(const std::string& filename, bool looped)
{
	if (m_musicSource)
		AudioManager::getInstance()->deleteSource(m_musicSource);
	m_musicSource = AudioManager::getInstance()->createSource(filename);
	reinterpret_cast<MiniAudioSource*>(m_musicSource)->play();
}

void MiniAudioMusicManager::stop()
{
	if (m_musicSource)
	{
		m_musicSource->stop();
		AudioManager::getInstance()->deleteSource(m_musicSource);
		m_musicSource = nullptr;
	}
}

bool MiniAudioMusicManager::isPlaying()
{
	if (m_musicSource)
		return m_musicSource->isPlaying();

	return false;
}

}