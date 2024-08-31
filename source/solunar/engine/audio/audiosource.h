#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

namespace solunar
{
	class AudioSource
	{
	public:
		virtual ~AudioSource() {}

		virtual bool IsPlaying();

		virtual void Play();
		virtual void Stop();
	};
}

#endif // !AUDIOSOURCE_H
