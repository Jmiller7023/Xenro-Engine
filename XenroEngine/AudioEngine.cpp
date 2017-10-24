#include "AudioEngine.h"
#include "ErrorMessages.h"
#include <iostream>
namespace Xenro{

void SFX::play(int loops) {
	m_channel = Mix_PlayChannel(-1, m_chunk, loops);
	//The audio channel failed to init.
	if (m_channel == -1) {
		//Overwrite channel 0 with SFX.
		if(Mix_PlayChannel(0, m_chunk, loops) == -1){
		errorMessage("failed to play SFX error: " + std::string(Mix_GetError()));
		}
	}
}

void Song::play(int loops) {

	if(Mix_PlayMusic(m_music, loops) == -1) {
		errorMessage("failed to play song error: " + std::string(Mix_GetError()));
	}
}

void Song::pause() {
	Mix_PauseMusic();
}

void Song::resume() {
	Mix_ResumeMusic();
}

void Song::stop() {
	Mix_HaltMusic();
}

AudioEngine::AudioEngine()

{
	openEngine();
}


AudioEngine::~AudioEngine()
{

}

SFX AudioEngine::loadSFX(const std::string& filePath) {
	SFX soundEffect;
	Mix_Chunk* Chunk;
	auto it = m_SFXMap.find(filePath);
	if (it == m_SFXMap.end()) {
		//wasn't in map, so add it to map.
		Chunk = Mix_LoadWAV(filePath.c_str());
		if (Chunk == nullptr) {
			fatalError("Failed to loadWAV: " + filePath);
		}

		m_SFXMap[filePath] = Chunk;

	} else {
		Chunk = it->second;
	}

	soundEffect.m_chunk = Chunk;
	return soundEffect;
}

Song AudioEngine::loadSong(const std::string& filePath) {
	Song song;
	Mix_Music* music;
	auto it = m_songMap.find(filePath);
	if (it == m_songMap.end()) {
		//wasn't in map, so add it to map.
		music = Mix_LoadMUS(filePath.c_str());
		if (music == nullptr) {
			fatalError("Failed to loadWAV: " + filePath);
		}

		m_songMap[filePath] = music;

	}
	else {
		music = it->second;
	}

	song.m_music = music;
	return song;
}

void AudioEngine::openEngine() {
	//Parameter is a combination of bitwise ors of the following flags.
	//MIX_INIT_FLAC, MIX_INIT_MOD, MIX_INIT_MP3 and MIX_INIT_OGG.
	//returns a -1 if it fails to init.
	if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) == -1) {
		fatalError("Failed to initialize Mixer!\n Mixer returned: " + std::string(Mix_GetError()));
	}

	//For third parameter, 1 enables mono and 2 enbales stereo.
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		fatalError("Failed to initialize Mixer!\n Mixer returned: " + std::string(Mix_GetError()));
	}
}

void AudioEngine::closeEngine() {
	//free the memory
	for (auto it = m_SFXMap.begin(); it != m_SFXMap.end(); it++) {
		Mix_FreeChunk(it->second);
	}

	for (auto it = m_songMap.begin(); it != m_songMap.end(); it++) {
		Mix_FreeMusic(it->second);
	}

	m_SFXMap.clear();
	m_songMap.clear();

	Mix_CloseAudio();
	Mix_Quit();
}

}