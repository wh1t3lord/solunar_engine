#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

namespace solunar
{
	class AudioSource;

	class MusicManager : public Singleton<MusicManager>
	{
	public:
		virtual void init() = 0;
		virtual void shutdown() = 0;

		virtual void play(const std::string& filename, bool looped = false) = 0;
		virtual void stop() = 0;

		virtual bool isPlaying() = 0;
	};
}

#endif // !MUSICMANAGER_H
