#ifndef MINIAUDIOSOURCE_H
#define MINIAUDIOSOURCE_H

#include "engine/audio/audiosource.h"

#include "miniaudiodrv/miniaudio_shared.h"

namespace solunar
{

class MiniAudioSource : public AudioSource
{
public:
	MiniAudioSource();
	MiniAudioSource(const std::string& filename);
	~MiniAudioSource();

	bool IsPlaying() override;

	void Play() override;
	void Stop() override;

	// MiniAudio stuff
	ma_uint64 process(float* pInput, float* pOutput, size_t frames);

	MiniAudioSource* clone();

private:
	ma_decoder m_decoder;
	ma_decoder_config m_config;
	// audio_device_format fmt;
};

}

#endif // !MINIAUDIOSOURCE_H
