#include "miniaudiodrv_pch.h"
#include "miniaudiodrv/miniaudiosource.h"

namespace solunar
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

	// Initialize decoder
	
	m_config = ma_decoder_config_init_default();
	ma_result result = ma_decoder_init_file(filename.c_str(), &m_config, &m_decoder);
	if (result != MA_SUCCESS) {
		Core::Error("MiniAudioSource: Failed to Initialize decoder. %s", ma_result_description(result));
	}
}

MiniAudioSource::~MiniAudioSource()
{
	ma_decoder_uninit(&m_decoder);

	// Clear
	memset(&m_decoder, 0, sizeof(m_decoder));
	memset(&m_config, 0, sizeof(m_config));
}

bool MiniAudioSource::IsPlaying()
{
	return false;
}

void MiniAudioSource::Play()
{
}

void MiniAudioSource::Stop()
{
}

ma_uint64 MiniAudioSource::process(float* pInput, float* pOutput, size_t frames)
{
	ma_uint64 framesRead = 0;
	ma_result result = ma_decoder_read_pcm_frames(&m_decoder, pOutput, frames, &framesRead);
	if (result != MA_SUCCESS) {
		Core::Msg("MiniAudioSource::process: Failed to read PCM frames! Error: %s", ma_result_description(result));
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