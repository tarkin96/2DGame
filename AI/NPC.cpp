#include "NPC.h"
#include "../gamedata.h"

//gets distance between two points
float NPC::dist(const Vector2f& npc, const Vector2f& player) {
  float x = npc[0]-player[0];
  float y = npc[1]-player[1];
  return hypot(x, y);
}

//finds direction of vector between two points
float NPC::dir(const Vector2f& npc, const Vector2f& player) {
	return atan2(npc[1] - player[1], npc[0] - player[0]);
}

//gets angle based on x and y value from origin
float NPC::calculateAngle(float x, float y) {
		return atan2(y , x);
}

NPC::NPC(const std::string& name, const Vector2f& player, int num) : 
	DirectionalSprite(name, 
		Vector2f(Gamedata::getInstance().getXmlInt("EnemyInfo/Pos/P" + std::to_string(num) + "/x"), 
		Gamedata::getInstance().getXmlInt("EnemyInfo/Pos/P" + std::to_string(num) + "/y")),
		Gamedata::getInstance().getXmlFloat("EnemyInfo/Pos/P" + std::to_string(num) + "/angle")),
	playerPosition(player),
	initPosition(
		Vector2f(Gamedata::getInstance().getXmlInt("EnemyInfo/Pos/P" + std::to_string(num) + "/x"), 
		Gamedata::getInstance().getXmlInt("EnemyInfo/Pos/P" + std::to_string(num) + "/y"))),
	initAngle(Gamedata::getInstance().getXmlFloat("EnemyInfo/Pos/P" + std::to_string(num) + "/angle")),
	playWidth(getScaledWidth()),
	playHeight(getScaledHeight()),
	sightRange(Gamedata::getInstance().getXmlInt("EnemyInfo/Stats/P" + std::to_string(num) + "/sightrange")),
	attackRange(Gamedata::getInstance().getXmlInt("EnemyInfo/Stats/P" + std::to_string(num) + "/attackrange")),
	currentAction(GUARD),
	moveSpeed(Gamedata::getInstance().getXmlInt(name+"/moveSpeed")),
	anim_timer(0),
	playing_anim(false),
	dead(false)
{
	
}

NPC::NPC(const std::string& name, const Vector2f& player, const Vector2f& pos, float initangle, 
	int pw, int ph, int sight, int attack_range) : 
		DirectionalSprite(name, pos, initangle),
		playerPosition(player),
		initPosition(pos),
		initAngle(initangle),
		playWidth(pw),
		playHeight(ph),
		sightRange(sight),
		attackRange(attack_range),
		currentAction(GUARD),
		moveSpeed(Gamedata::getInstance().getXmlInt(name+"/moveSpeed")),
		anim_timer(0),
		playing_anim(false),
		dead(false)
{
	
}

//THIS SECTION HAS ANIMATION STUFF
bool NPC::check_anim() const {
	return playing_anim;
}

void NPC::start_anim() {
	playing_anim = true;
	anim_timer = 0;
}

void NPC::update_anim() {
	anim_timer++;
	if (anim_timer > 20) {
		playing_anim = false;
	}
}
//END ANIMATION STUFF

void NPC::setPlayerPosition(const Vector2f& pos) {
	playerPosition = pos;
}

Vector2f NPC::getPlayerPosition() const {
	return playerPosition;
}

void NPC::update(Uint32 ticks) {
	DirectionalSprite::update(ticks);
	
	float far = dist(getPosition(), playerPosition);
	
	if (!check_anim()) {
		//while guarding
		if (currentAction == GUARD) {
			if (far < sightRange) {
				currentAction = CHASE;
				chase();
				//std::cout << "set to chase mode" << std::endl;
			}
			else {guard(); /*setAngle(initAngle)*/;}
		}
		//while chasing the player
		else if (currentAction == CHASE) {
			if (far > sightRange) {
				fallback();
			}
			else if((getPosition() == playerPosition) || fabs(dist(playerPosition, getPosition())) < attackRange) {
				attack();
			}
			else {chase();}
		}
		//while falling back to original position
		else if (currentAction == FALLBACK) {
			if ((getPosition() == initPosition) || fabs(dist(initPosition, getPosition())) < 2) {
				setPosition(initPosition);
				guard();
			}
			else if (far < sightRange) {
				chase();
			}
			else {fallback();}
		}
		//while attacking the player
		else if (currentAction == ATTACK) {
			if (far > sightRange) {
				fallback();
			}
			else if((getPosition() == playerPosition) || fabs(dist(playerPosition, getPosition())) < attackRange) {
				attack();
			}
			else {chase();}	
		}		
	}
	else {update_anim();}
	//Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	//if (incr != Vector2f(0, 0)) {advanceFrame(ticks);}
	//setPosition(getPosition() + incr);
	
}

void NPC::draw() const {
	if (getExplosion() || !dead) {
		DirectionalSprite::draw();
	}
}

void NPC::guard() {
	currentAction = GUARD;
	setVelocity(Vector2f(0.0, 0.0));
	setAngle(initAngle);
}

void NPC::chase() {
	currentAction = CHASE;
	float newangle = dir(playerPosition, getPosition());
	setVelocity(Vector2f(cos(newangle) * moveSpeed, sin(newangle) * moveSpeed));
	setAngle(calculateAngle(getVelocityX(), getVelocityY()) * 180/M_PI);
}

void NPC::fallback() {
	currentAction = FALLBACK;
	float newangle = dir(initPosition, getPosition());
	setVelocity(Vector2f(cos(newangle) * moveSpeed, sin(newangle) * moveSpeed));
	setAngle(calculateAngle(getVelocityX(), getVelocityY()) * 180/M_PI);
}

void NPC::attack() {
	currentAction = ATTACK;
	start_anim();
	setVelocity(Vector2f(0.0, 0.0));
}

bool NPC::isDead() const {return dead;}

void NPC::setDead(bool val) {dead = val;}
