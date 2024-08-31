#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

namespace solunar
{
	class AudioSource;

	class MusicManager : public Singleton<MusicManager>
	{
	public:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void Play(const std::string& filename, bool looped = false) = 0;
		virtual void Stop() = 0;

		virtual bool IsPlaying() = 0;
	};
}

#endif // !MUSICMANAGER_H
