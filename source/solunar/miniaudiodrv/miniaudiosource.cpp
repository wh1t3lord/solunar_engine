#include "miniaudiodrv_pch.h"
#include "miniaudiodrv/miniaudiosource.h"

namespace engine
{

MiniAudioSource::MiniAudioSource()
{
	// Clear
	memset(&m_decoder, 0, sizeof(m_decoder));
	memset(&m_config, 0, sizeof(m_config));
}

MiniAudioSource::MiniAudioSource(const std::string& filename)
{
	// Clear
	memset(&m_decoder, 0, sizeof(m_decoder));
	memset(&m_config, 0, sizeof(m_config));

	// #TODO: make decoder config based on device values
	// https://github.com/vertver/audiorex/blob/master/include/emitters_system.h

	// initialize decoder
	
	m_config = ma_decoder_config_init_default();
	ma_result result = ma_decoder_init_file(filename.c_str(), &m_config, &m_decoder);
	if (result != MA_SUCCESS) {
		Core::error("MiniAudioSource: Failed to initialize decoder. %s", ma_result_description(result));
	}
}

MiniAudioSource::~MiniAudioSource()
{
	ma_decoder_uninit(&m_decoder);

	// Clear
	memset(&m_decoder, 0, sizeof(m_decoder));
	memset(&m_config, 0, sizeof(m_config));
}

bool MiniAudioSource::isPlaying()
{
	return false;
}

void MiniAudioSource::play()
{
}

void MiniAudioSource::stop()
{
}

ma_uint64 MiniAudioSource::process(float* pInput, float* pOutput, size_t frames)
{
	ma_uint64 framesRead = 0;
	ma_result result = ma_decoder_read_pcm_frames(&m_decoder, pOutput, frames, &framesRead);
	if (result != MA_SUCCESS) {
		Core::msg("MiniAudioSource::process: Failed to read PCM frames! Error: %s", ma_result_description(result));
	}

	return framesRead;
}

MiniAudioSource* MiniAudioSource::clone()
{
	MiniAudioSource* miniAudioSource = mem_new<MiniAudioSource>();
	miniAudioSource->m_config = m_config;
	miniAudioSource->m_decoder = m_decoder;
	return miniAudioSource;
}

}