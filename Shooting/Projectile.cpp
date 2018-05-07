
#include "Projectile.h"
#include "../gamedata.h"

Projectile::Projectile(const std::string& name, const Vector2f& pos, const Vector2f& vel) :
	DirectionalSprite("ProjectileInfo/" + name,
		pos, vel, 0.0),
	damage(Gamedata::getInstance().getXmlFloat("ProjectileInfo/" + name + "/Damage")),
	speed(Gamedata::getInstance().getXmlFloat("ProjectileInfo/" + name + "/Speed")),
	range(Gamedata::getInstance().getXmlFloat("ProjectileInfo/" + name + "/Range")),
	origRange(),
	penetrate(Gamedata::getInstance().getXmlFloat("ProjectileInfo/" + name + "/Penetrate")),
	origPen(),
	startLife(),
	lifeSpan(Gamedata::getInstance().getXmlFloat("ProjectileInfo/" + name + "/LifeSpan"))
{
	origRange = range;
	origPen = penetrate;
	gettimeofday(&startLife, NULL);
}

Projectile::Projectile(const std::string& name) :
	DirectionalSprite("ProjectileInfo/" + name, Vector2f(0.0, 0.0), Vector2f(0.0, 0.0), 0.0),
	damage(Gamedata::getInstance().getXmlFloat("ProjectileInfo/" + name + "/Damage")),
	speed(Gamedata::getInstance().getXmlFloat("ProjectileInfo/" + name + "/Speed")),
	range(Gamedata::getInstance().getXmlFloat("ProjectileInfo/" + name + "/Range")),
	origRange(),
	penetrate(Gamedata::getInstance().getXmlFloat("ProjectileInfo/" + name + "/Penetrate")),
	origPen(),
	startLife(),
	lifeSpan(Gamedata::getInstance().getXmlFloat("ProjectileInfo/" + name + "/LifeSpan"))
{
	origRange = range;
	origPen = penetrate;
	gettimeofday(&startLife, NULL);
	//std::cout << "created" << std::endl;
}

float Projectile::getSpeed() {
	return speed;
}

void Projectile::update(Uint32 ticks) {
	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	range = range - pow((incr[0] * incr[0]) + (incr[1] * incr[1]), 0.5);
	setPosition(getPosition() + incr);
	if (incr != Vector2f(0, 0)) {advanceFrame(ticks);}
	else {
		setCurrentFrame(0);
	}
	//std::cout << "updated" << std::endl;
}

void Projectile::draw() const {
	getImage()->drawAngled(getX(), getY(), getScale(), getAngle());
}

int Projectile::getPenetrate() const {
	return penetrate;
}

void Projectile::setPenetrate(int pen) {
	penetrate = pen;
}

float Projectile::getRange() const {
	return range;
}

void Projectile::setRange(float r) {
	range = r;
}

struct timeval Projectile::getStartLife() const {
	return startLife;
}

void Projectile::setStartLife(struct timeval t) {
	startLife = t;
}

float Projectile::getLifeSpan() const {
	return lifeSpan;
}

void Projectile::setLifeSpan(float l) {
	lifeSpan = l;
}

void Projectile::reset() {
	range = origRange;
	penetrate = origPen;
	gettimeofday(&startLife, NULL);
}
