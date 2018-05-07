#include <string>
#include <iostream>
#include <ctime>
#include "sound.h"
using std::string;

Sound::~Sound() {
	Mix_FreeChunk(sound);
}

int Sound::getVolume() {
	return volume;
}

Mix_Chunk* Sound::getSound() {
	return sound;
}

Sound::Sound(Mix_Chunk* chunk, int vol) :
	sound(chunk),
	volume(vol)
{
	
}

void Sound::play() {
	
}
