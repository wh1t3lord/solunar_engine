#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

namespace solunar
{
	class AudioSource;
	
	class AudioManager : public Singleton<AudioManager>
	{
	protected:
		static AudioManager* ms_pInstance;
	public:
		static AudioManager* createInstance();
		static void destroyInstance();
	public:
		AudioManager();
		virtual ~AudioManager();

		virtual void init();
		virtual void shutdown();

		virtual void update();

		virtual AudioSource* createSource(const std::string& filename);
		virtual void deleteSource(AudioSource* source);

		virtual void setListenerPosition(const glm::vec3& position);
	};
}

#endif // !AUDIOMANAGER_H
