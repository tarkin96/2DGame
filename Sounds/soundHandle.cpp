#include "soundHandle.h"
#include "../gamedata.h"
#include <string>
#include <iostream>
#include <ctime>

SoundHandle& SoundHandle::getInstance() {
  static SoundHandle soundhandle;
  return soundhandle;
}

SoundHandle::~SoundHandle() {
	std::cout << "Cleaning up sounds ..." << std::endl;
	Mix_HaltMusic();
	Mix_FreeMusic(music);
	std::map<std::string, Sound*>::iterator it = sounds.begin();
	while (it != sounds.end()) {
		delete it->second;
		it = sounds.erase(it);
	}
	Mix_CloseAudio();
}

SoundHandle::SoundHandle() : 
	volume(SDL_MIX_MAXVOLUME/4), 
	currentSound(), 
	music(NULL),
	audioRate(22050), 
	audioChannels(2), 
	audioBuffers(2048),
	sounds(),
	channels() 
{

	if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels, 
		audioBuffers)){
		throw std::string("Unable to open audio!");
	}
	
	music = Mix_LoadMUS(Gamedata::getInstance().getXmlStr("Sounds/Music/filename").c_str());
	startMusic();
	for(int s = 0; s < Gamedata::getInstance().getXmlInt("Sounds/SoundQuantity"); s++) {
		sounds[Gamedata::getInstance().getXmlStr("Sounds/S" + std::to_string(s) + "/name")] = new Sound(Mix_LoadWAV(Gamedata::getInstance().getXmlStr("Sounds/S" + std::to_string(s) + "/filename").c_str()),
			Gamedata::getInstance().getXmlInt("Sounds/S" + std::to_string(s) + "/volume"));
		channels[(Gamedata::getInstance().getXmlStr("Sounds/S" + std::to_string(s) + "/name"))] = s;
	}
}

void SoundHandle::toggleMusic() {
	if( Mix_PausedMusic() ) { 
		Mix_ResumeMusic(); 
	} 
	else { 
		Mix_PauseMusic(); 
	} 
}

void SoundHandle::operator[](const std::string& index) {
	currentSound = index;
	Mix_VolumeChunk(sounds[index]->getSound(), sounds[index]->getVolume());
	channels[index] = 
	Mix_PlayChannel(-1, sounds[index]->getSound(), 0);
}

void SoundHandle::startMusic() {
	Mix_VolumeMusic(Gamedata::getInstance().getXmlInt("Sounds/Music/volume"));

	Mix_PlayMusic(music, -1);
}

void SoundHandle::stopMusic() {
	Mix_HaltMusic();
	Mix_FreeMusic(music);
}

