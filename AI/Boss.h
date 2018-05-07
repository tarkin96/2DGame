/*	This class represents the boss in the scene. */

#ifndef BOSS_H
#define BOSS_H
#include "NPC.h"

class Player;

class Boss : public NPC {
	public:
		Boss(const std::string&, const Vector2f&, Player*);
		virtual void update(Uint32);
		virtual void draw() const;
		
		//Boss Tactics
		void spawnWave();
		void spawnOther();
		void spit();
		
	private:
		Player * player;
		int waveSize;
		int aggroRange;
		float waveTimer;
		float summonTimer;
		//std::vector<NPC *> minions;
		time_t startTime;
		time_t lastWave;
		
		Boss(const Boss&) = delete;
		Boss& operator=(const Boss&) = delete;
};
#endif
