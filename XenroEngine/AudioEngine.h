/*************************************************************************/
/*                           AudioEngine.h                               */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Xenro Engine                                */
/*             https://github.com/Jmiller7023/Xenro-Engine               */
/*************************************************************************/
/*                 Copyright  11-3-2017 Joseph Miller.                   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

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
	void playUntilEffectFinishes(int loops = 0);
	void waitUntilEffectsFinish();

private:
	Mix_Chunk* m_chunk = nullptr;
	//set to a channel that can't be accessed.
	int m_channel = -3;
	int m_volume = MIX_MAX_VOLUME;
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
	int m_volume = MIX_MAX_VOLUME;
};

class AudioEngine {
public:
	AudioEngine();
	~AudioEngine();

	SFX loadSFX(const std::string& filePath);
	SFX AudioEngine::loadSFX(const std::string& filePath, int volume);
	Song loadSong(const std::string& filePath);
	Song AudioEngine::loadSong(const std::string& filePath, int volume);

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