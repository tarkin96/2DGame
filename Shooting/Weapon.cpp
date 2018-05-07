#include "Weapon.h"
#include "../gamedata.h"
#include "../viewport.h"
#include "../Sounds/soundHandle.h"
#include <ios>

Weapon::Weapon(const std::string& name, bool godly) :
	//proj(new Projectile(Gamedata::getInstance().getXmlStr("Weapons/TypeDefs/" + name + "/ProjectileType"))),
	activeProj(),
	projectiles(),
	currActiveProj(0),
	currProj(0),
	activeProjCount(0),
	ammo(Gamedata::getInstance().getXmlFloat("Weapons/TypeDefs/" + name + "/StartAmmo")),
	arcCount(Gamedata::getInstance().getXmlFloat("Weapons/TypeDefs/" + name + "/Arc")),
	arcSize(Gamedata::getInstance().getXmlFloat("Weapons/TypeDefs/" + name + "/ArcSize")),
	nameType(name),
	sound(Gamedata::getInstance().getXmlStr("Weapons/TypeDefs/" + name + "/Sound")),
	god(godly)
{
	//create pool of projectiles for first weapon
	std::string projname = Gamedata::getInstance().getXmlStr("Weapons/TypeDefs/" + name + "/ProjectileType");
	projectiles.reserve(Gamedata::getInstance().getXmlInt("ProjectileInfo/" + projname + "/PoolSize") * arcCount);
	activeProj.resize(Gamedata::getInstance().getXmlInt("ProjectileInfo/" + projname + "/PoolSize"));
	for (int t = 0; t < Gamedata::getInstance().getXmlInt("ProjectileInfo/" + projname + "/PoolSize"); t++) {
		projectiles.emplace_back(Projectile(projname));
	}
}

Weapon::~Weapon() {
	//delete proj;
}

const std::vector<Projectile>& Weapon::getProjectiles() const {
	return projectiles;
}

const std::vector<Projectile*>& Weapon::getActiveProj() const {
	return activeProj;
}

const std::string& Weapon::getName() const {
	return nameType;
}


void Weapon::update(Uint32 ticks) {
	struct timeval currTime;
	gettimeofday(&currTime, NULL);
	
	double milli = (double)currTime.tv_usec / 1000000.0;
	double sec = (double)currTime.tv_sec;
	for (Projectile*& proj : activeProj) {
		
		if (proj) {
			double projmilli = (double)proj->getStartLife().tv_usec / 1000000.0;
			double projsec = (double)proj->getStartLife().tv_sec;
			proj->update(ticks);
	
			if (proj->getX() > proj->getWorldWidth() || proj->getX() < 0.0 - proj->getScaledWidth()) {
				activeProjCount--;
				proj = NULL;
			}
			else if (proj->getY() > proj->getWorldHeight() || proj->getY() < 0.0 - proj->getScaledHeight()) {
				activeProjCount--;
				proj = NULL;
			}
			else if (proj->getPenetrate() < 0) {
				activeProjCount--;
				proj = NULL;
			}
			else if ((sec + milli) - (projsec + projmilli) >= proj->getLifeSpan()) {
				activeProjCount--;
				proj=NULL;
			}
			
		}

	}
	
}


void Weapon::draw() const {
	//draw the active projectiles
	for (Projectile * proj : activeProj) {
		if (proj) {
			if (Viewport::getInstance().check(proj)) {
				proj->draw();
			}
		}
	}
}

void Weapon::shoot(float angle, const Vector2f& pos, const Vector2f& vel, bool godMode) {
	//allow shootign if player is not exploding
	if ((ammo > 0)) {
		if (!godMode) {
			ammo--;
		}
		float newangle;
		if (arcCount > 1) {
			newangle = angle + (arcSize / 2.0);
		}
		else {newangle = angle;}
			
		SoundHandle::getInstance()[sound];
		for (int i = 0; i < arcCount; i++) {
			
			if (i > 0) {
				newangle -= (arcSize / (float)arcCount);
			}
			
			if (activeProjCount < projectiles.size()) {
				activeProjCount++;
			}
			
			if (currActiveProj >= activeProj.size()) {
				currActiveProj = 0;
				currProj = 0;
			}
			
			//create projectile and give it a speed/direction
			activeProj[currActiveProj] = &projectiles[currProj];
			projectiles[currProj].setAngle(newangle);
			Vector2f newoffset(0.0, 0.0);
			projectiles[currProj].setPosition(pos + newoffset);
			projectiles[currProj].reset();

			//create new veolicty based on angle and speed
			if (newangle >= 0 && newangle <= 90) {
				projectiles[currProj].setVelocity(Vector2f(
					projectiles[currProj].getSpeed() * sin((90.0 - newangle) / (180/M_PI)) + vel[0],
					projectiles[currProj].getSpeed() * sin(newangle  / (180/M_PI)) + vel[1]));
			}
			else if (newangle > 90 && newangle <= 180) {
				projectiles[currProj].setVelocity(Vector2f(
					projectiles[currProj].getSpeed() * sin((90.0 - newangle) / (180/M_PI)) + vel[0],
					projectiles[currProj].getSpeed() * sin(newangle  / (180/M_PI)) + vel[1]));
			}
			else if (newangle < 0 && newangle >= -90.0) {
				projectiles[currProj].setVelocity(Vector2f(
					projectiles[currProj].getSpeed() * sin((90.0 + newangle) / (180/M_PI)) + vel[0],
					projectiles[currProj].getSpeed() * sin(newangle / (180/M_PI)) + vel[1]));
			}
			else {
				projectiles[currProj].setVelocity(Vector2f(
					projectiles[currProj].getSpeed() * sin((-270.0 - newangle) / (180/M_PI)) + vel[0],
					projectiles[currProj].getSpeed() * sin(newangle  / (180/M_PI)) + vel[1]));
			}
			projectiles[currProj].setCurrentFrame(0);
			currActiveProj++;
			currProj++;
		}
	}

}
