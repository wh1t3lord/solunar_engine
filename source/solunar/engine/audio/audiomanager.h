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
		static AudioManager* CreateInstance();
		static void DestroyInstance();
	public:
		AudioManager();
		virtual ~AudioManager();

		virtual void Init();
		virtual void Shutdown();

		virtual void Update();

		virtual AudioSource* CreateSource(const std::string& filename);
		virtual void DeleteSource(AudioSource* source);
		
		virtual void setListenerPosition(const glm::vec3& position);

	};
}

#endif // !AUDIOMANAGER_H
