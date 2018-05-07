#include "Boss.h"
#include "../Player.h"
#include "../gamedata.h"

Boss::Boss(const std::string& name, const Vector2f& playerPos, Player * playerObj) : 
	NPC(name, playerPos, -1),
	player(playerObj),
	waveSize(Gamedata::getInstance().getXmlInt("BossTactics/WaveSize")),
	aggroRange(Gamedata::getInstance().getXmlInt("BossTactics/AggroRange")),
	waveTimer(Gamedata::getInstance().getXmlInt("BossTactics/WaveTimer")),
	summonTimer(Gamedata::getInstance().getXmlInt("BossTactics/SummonTimer")),
	startTime(),
	lastWave()
{
	time(&startTime);
	time(&lastWave);
}

void Boss::update(Uint32 ticks) {
	NPC::update(ticks);
	if (!isDead()) {
		time_t timeKeeper;
		time(&timeKeeper);
		
		//perform tactics
		if (difftime(timeKeeper, startTime) > summonTimer) {
			spawnOther();
			startTime = timeKeeper;
		}
		if (difftime(timeKeeper, lastWave) > waveTimer && dist(getPosition(), getPlayerPosition()) < aggroRange) {
			spawnWave();
			lastWave = timeKeeper;
		}
	}

}

void Boss::draw() const {
	NPC::draw();
	
}

void Boss::spawnOther() {
	Vector2f randompos(rand()%getWorldWidth(), rand()%getWorldHeight());
	if (randompos[0] > 0 && randompos[0] < getWorldWidth()) {
		randompos[1] = (rand()%2)?0:getWorldHeight();
	}
	float randomangle = rand()%360;
	player->attach(new NPC("Enemy", getPlayerPosition(), randompos, randomangle, player->getScaledHeight(), player->getScaledWidth(), 9999, 20));
}

void Boss::spawnWave() {
	for (int i = 0; i < waveSize; i++) {
		Vector2f pos(getWorldWidth(), rand()%getWorldHeight());
		player->attach(new NPC("TickleElmo", getPlayerPosition(), pos, 180, player->getScaledHeight(), player->getScaledWidth(), 9999, 5));
	}
}

void Boss::spit() {
	
}
