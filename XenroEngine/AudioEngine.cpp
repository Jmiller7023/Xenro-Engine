/*************************************************************************/
/*                         AudioEngine.cpp                               */
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

#include "AudioEngine.h"
#include "ErrorMessages.h"
#include <iostream>

namespace Xenro{

void SFX::play(int loops) {

	//Set chunk sound level.
	Mix_VolumeChunk(m_chunk, m_volume);

	m_channel = Mix_PlayChannel(-1, m_chunk, loops);
	//The audio channel failed to init.
	if (m_channel == -1) {
		//Overwrite channel 0 with SFX.
		if(Mix_PlayChannel(0, m_chunk, loops) == -1){
		warning("failed to play SFX error: " + std::string(Mix_GetError()));
		}
	}
}

void SFX::playUntilEffectFinishes(int loops) {

	//Set chunk sound level.
	Mix_VolumeChunk(m_chunk, m_volume);

	m_channel = Mix_PlayChannel(-1, m_chunk, loops);
	//The audio channel failed to init.
	if (m_channel == -1) {
		//Overwrite channel 0 with SFX.
		if (Mix_PlayChannel(0, m_chunk, loops) == -1) {
			warning("failed to play SFX error: " + std::string(Mix_GetError()));
		}
	}
	while (Mix_Playing(m_channel));
}

void SFX::waitUntilEffectsFinish() {
	while (Mix_Playing(-1) > 0);
}

void Song::play(int loops) {

	//set song volume
	Mix_VolumeMusic(m_volume);

	if(Mix_PlayMusic(m_music, loops) == -1) {
		warning("failed to play song error: " + std::string(Mix_GetError()));
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

void Song::fadeInSong(int loops, int ms) {
	if (Mix_FadeInMusic(m_music, loops, ms) == -1) {
		warning("Mix_FadeInMusic: " + std::string(Mix_GetError()));
	}
}

void Song::fadeOutSong(int ms) {
	if (Mix_FadeOutMusic(ms) == -1) {
		warning("Mix_FadeOutMusic: " + std::string(Mix_GetError()));
	}
}

int Song::getCurrVolume() {
	return Mix_VolumeMusic(-1);
}

void Song::playAtTime(double s, int loops) {

	//set song volume
	Mix_VolumeMusic(m_volume);

	if (Mix_PlayMusic(m_music, loops) == -1) {
		warning("failed to play song error: " + std::string(Mix_GetError()));
	}

	Mix_MusicType type = Mix_GetMusicType(NULL);
	if (type == Mix_MusicType::MUS_MP3) {
		Mix_RewindMusic();
		if (Mix_SetMusicPosition(s) == -1) {
			printf("Mix_SetMusicPosition: %s\n", Mix_GetError());
		}
	}
	else if (type == Mix_MusicType::MUS_OGG || type == Mix_MusicType::MUS_MOD) {
		if (Mix_SetMusicPosition(s) == -1) {
			printf("Mix_SetMusicPosition: %s\n", Mix_GetError());
		}
	} else {
		warning("To start a song at a specific time, it must be of type OGG or MOD");
	}
}

void AudioEngine::printCurrSongType() {

	switch (Mix_GetMusicType(NULL)) {
		case MUS_NONE:
			printf("No music is playing");
		case MUS_CMD:
			printf("Command based music is playing.\n");
			break;
		case MUS_WAV:
			printf("WAVE/RIFF music is playing.\n");
			break;
		case MUS_MOD:
			printf("MOD music is playing.\n");
			break;
		case MUS_MID:
			printf("MIDI music is playing.\n");
			break;
		case MUS_OGG:
			printf("OGG music is playing.\n");
			break;
		case MUS_MP3:
			printf("MP3 music is playing.\n");
			break;
		default:
			printf("Unknown music is playing.\n");
			break;
	}
}

Songtype AudioEngine::getCurrSongType() {
	switch (Mix_GetMusicType(NULL)) {
		case MUS_NONE:
			return Songtype::NONE;
		case MUS_CMD:
			return Songtype::CMD;
			break;
		case MUS_WAV:
			return Songtype::WAV;
			break;
		case MUS_MOD:
			return Songtype::MOD;
			break;
		case MUS_MID:
			return Songtype::MID;
			break;
		case MUS_OGG:
			return Songtype::OGG;
			break;
		case MUS_MP3:
			return Songtype::MP3;
			break;
		default:
			return Songtype::UNKNOWN;
			break;
	}
}

bool AudioEngine::isSongPlaying() {
	if (Mix_PlayingMusic())
		return true;
	return false;
}

bool AudioEngine::isSongPaused() {
	if (Mix_PausedMusic())
		return true;
	return false;
}

void AudioEngine::RestartCurrSong() {
	Songtype type = getCurrSongType();
	if (type == Songtype::MP3 || type == Songtype::OGG || type == Songtype::MID || type == Songtype::MOD) {
		// rewind music playback to the start
		Mix_RewindMusic();
	}
	else {
		warning("To Restart a Song, a song must be playing of type OGG, MIDI, or MOD");
	}
}

AudioEngine::AudioEngine() {
	openEngine();
}

AudioEngine::~AudioEngine() {
	closeEngine();
}

SFX AudioEngine::loadSFX(const std::string& filePath) {
	SFX soundEffect;
	Mix_Chunk* Chunk;
	auto it = m_SFXMap.find(filePath);
	if (it == m_SFXMap.end()) {
		//wasn't in map, so add it to map.
		Chunk = Mix_LoadWAV(filePath.c_str());
		if (Chunk == nullptr) {
			fatalError("Failed to loadSFX: " + filePath);
		}

		Mix_VolumeChunk(Chunk, MIX_MAX_VOLUME);
		m_SFXMap[filePath] = Chunk;

	} else {
		Chunk = it->second;
	}

	soundEffect.m_chunk = Chunk;
	return soundEffect;
}

SFX AudioEngine::loadSFX(const std::string& filePath, int volume) {

	SFX soundEffect;

	Mix_Chunk* Chunk;
	auto it = m_SFXMap.find(filePath);
	if (it == m_SFXMap.end()) {
		//wasn't in map, so add it to map.
		Chunk = Mix_LoadWAV(filePath.c_str());
		if (Chunk == nullptr) {
			fatalError("Failed to loadSFX: " + filePath);
		}

		m_SFXMap[filePath] = Chunk;

	}
	else {
		Chunk = it->second;
	}

	soundEffect.m_volume = volume;
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
			fatalError("Failed to loadSONG: " + filePath);
		}

		m_songMap[filePath] = music;

	}
	else {
		music = it->second;
	}

	song.m_music = music;
	return song;
}

Song AudioEngine::loadSong(const std::string& filePath, int volume) {
	Song song;
	Mix_Music* music;
	auto it = m_songMap.find(filePath);
	if (it == m_songMap.end()) {
		//wasn't in map, so add it to map.
		music = Mix_LoadMUS(filePath.c_str());
		if (music == nullptr) {
			fatalError("Failed to loadSONG: " + filePath);
		}
		m_songMap[filePath] = music;
	}
	else {
		music = it->second;
	}

	song.m_volume = volume;
	song.m_music = music;
	return song;
}

void AudioEngine::openEngine() {
	//Parameter is a combination of bitwise ors of the following flags.
	//MIX_INIT_FLAC, MIX_INIT_MOD, MIX_INIT_MP3 and MIX_INIT_OGG.
	//returns a -1 if it fails to init.
	if (Mix_Init(MIX_INIT_OGG | MIX_INIT_FLAC | MIX_INIT_MOD) == -1) {
		fatalError("Failed to initialize Mixer!\n Mixer returned: " + std::string(Mix_GetError()));
	}

	//For third parameter, 1 enables mono and 2 enbales stereo.
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_S16SYS, 2, 4096) == -1) {
		fatalError("Failed to initialize Mixer!\n Mixer returned: " + std::string(Mix_GetError()));
	}
}

void AudioEngine::closeEngine() {
	if (!EngineClosed) {
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
		EngineClosed = true;
	}
}

}