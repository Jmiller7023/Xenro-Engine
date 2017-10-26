#ifndef XENRO_AUDIOENGINE_DEFINED
#define XENRO_AUDIOENGINE_DEFINED

#include <SDL/SDL_mixer.h>
#include <string>
#include <map>
namespace Xenro{

	class SFX {
	public:
		friend class AudioEngine;
		void play(int loops = 0);

	private:
		Mix_Chunk* m_chunk = nullptr;
		//set to a channel that can't be accessed.
		int m_channel = -3;
};

	class Song {
	public:
		friend class AudioEngine;
		void play(int loops = -1);

		static void pause();

		static void resume();

		static void stop();
	private:
		Mix_Music* m_music = nullptr;
	};

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();

	SFX loadSFX(const std::string& filePath);
	Song loadSong(const std::string& filePath);

	void openEngine();
	//Meant to be called when all audioEngine is finished.
	void closeEngine();

private:
	float m_temp = 100.0f;
	std::map<std::string, Mix_Chunk*> m_SFXMap;
	std::map<std::string, Mix_Music*> m_songMap;
};



}

#endif //XENRO_AUDIOENGINE_DEFINED