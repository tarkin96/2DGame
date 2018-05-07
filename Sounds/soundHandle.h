#ifndef SOUNDHAND_H
#define SOUNDHAND_H
#include "sound.h"
class SoundHandle {
	public:
		static SoundHandle& getInstance();
		
		~SoundHandle();
		void operator[](const std::string&);
		
		void startMusic();
		void stopMusic();      // stop all sounds
		void toggleMusic();    // toggle music on/off
	private:
		SoundHandle();
		int volume;
		std::string currentSound;
		Mix_Music *music;

		int audioRate;
		int audioChannels;
		int audioBuffers;
		std::map<std::string, Sound*> sounds; //map of sounds (indexed by name)
		std::map<std::string, int> channels;  //map of audio channels (indexed by sound name)
		SoundHandle(const SoundHandle&);
		SoundHandle& operator=(const SoundHandle&);
};

#endif
