#ifndef DIRECTIONALSPRITE__H
#define DIRECTIONALSPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "Explosions/Explosion.h" 

class DirectionalSprite : public Drawable {
public:
	DirectionalSprite(const std::string&);
	DirectionalSprite(const std::string&, const Vector2f&, float);
	DirectionalSprite( const std::string&, const Vector2f&, const Vector2f&, float);
	DirectionalSprite(const DirectionalSprite&);
	~DirectionalSprite();

	virtual void draw() const;
	virtual void update(Uint32 ticks);

	//getters and setters
	virtual const Image* getImage() const { 
		return images[currentFrame]; 
	}
	int getScaledWidth()  const { 
		return getScale()*images[currentFrame]->getWidth(); 
	} 
	int getScaledHeight()  const { 
		return getScale()*images[currentFrame]->getHeight(); 
	} 
	float getAngle() const {
	  return angle;
	}
	void setAngle(float newangle) {
	  angle = newangle;
	}
	
	int getWorldWidth() const {
	  return worldWidth;
	}
	
	int getWorldHeight() const {
	  return worldHeight;
	}
	
	virtual const SDL_Surface* getSurface() const { 
		return images[currentFrame]->getSurface();
	}

	void setCurrentFrame(int val) {currentFrame = val;}
	
	int getCurrentFrame() const {return currentFrame;}
	
	//get the pointer to the explosion object
	const Explosion * getExplosion() const {return explosion;}
	
	void advanceFrame(Uint32 ticks);
	DirectionalSprite& operator=(const DirectionalSprite&);
	
	//EXPLOSION STUFF
	void explode();

private:
	std::vector<Image *> images;
	float angle;
	unsigned currentFrame;
	unsigned numberOfFrames;
	unsigned frameInterval;
	float timeSinceLastFrame;
	int worldWidth;
	int worldHeight;
	
	//EXPLOSION STUFF
	Explosion * explosion;
};
#endif
