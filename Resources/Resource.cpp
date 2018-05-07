#include "Resource.h"

Resource::Resource(const std::string& name, const Vector2f& pos) : 
	Sprite(name, pos, Vector2f(0.0, 0.0)),
	playerPos(),
	explosion(nullptr),
	destroyed(false)
{
	
}

Resource::~Resource() {
	delete explosion;
}

const Explosion * Resource::getExplosion() const {
	return explosion;
}

void Resource::explode() {
	if ( !explosion ) explosion = new Explosion(*this); // make sure not exploding
	destroyed = true; //object is now destroyed
}

void Resource::update(Uint32 ticks) {
	if ( explosion ) { //only update the explosion
		explosion->update(ticks);
		if ( explosion->chunkCount() == 0 ) {
			delete explosion;
			explosion = NULL;
		}
	}
}

void Resource::draw() const {
		if (!explosion && !destroyed) {
			getImage()->draw(getX(), getY(), getScale()); //draw normal image
		}
		else if (explosion && destroyed) {explosion->draw();}	//draw exploded image
}

void Resource::setPlayerPosition(const Vector2f& pos) {
	playerPos = pos;
}

bool Resource::isDestroyed() {return destroyed;}
