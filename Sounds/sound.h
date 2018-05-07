#ifndef SOUND_H
#define SOUND_H
#include <vector>
#include <map>
#include <iostream>
#include <time.h>
#include <SDL.h>
#include <SDL/SDL_mixer.h>

class Sound {
	public:
		Sound(Mix_Chunk *, int);
		~Sound();
		Mix_Chunk * getSound();
		int getVolume();
		void play();
	private:
		Mix_Chunk * sound;
		int volume;
		Sound(const Sound&) = delete;
		Sound operator=(const Sound&) = delete;
};

#endif
