//This file handles the directional sprites. It can be oriented by passing an
//angle to it's image draw. 

#include "DirectionalSprite.h"
#include "gamedata.h"
#include "renderContext.h"
#include "Sounds/soundHandle.h"

//directional sprites can have multiple frames
void DirectionalSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

DirectionalSprite::DirectionalSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance()->getImages(name) ),
  angle(),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  explosion(nullptr)
{ }

DirectionalSprite::DirectionalSprite( const std::string& name, const Vector2f& pos, float degree) :
  Drawable(name, Vector2f(pos), Vector2f(0.0, 0.0)),
  images( RenderContext::getInstance()->getImages(name) ),
  angle(degree),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  explosion(nullptr)
{ }

DirectionalSprite::DirectionalSprite( const std::string& name, const Vector2f& pos, const Vector2f& vel, float degree) :
  Drawable(name, pos, vel),
  images( RenderContext::getInstance()->getImages(name) ),
  angle(degree),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  explosion(nullptr)
{ }

DirectionalSprite::DirectionalSprite(const DirectionalSprite& s) :
  Drawable(s), 
  images(s.images),
  angle(s.getAngle()),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  explosion(nullptr)
  { }

DirectionalSprite& DirectionalSprite::operator=(const DirectionalSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}

DirectionalSprite::~DirectionalSprite() {
	delete explosion;
}

//draw
void DirectionalSprite::draw() const { 
	
	//draws the explosion or the current frame
	if (explosion) {
		explosion->draw(); //draw explosion
	}
	else {
		images[currentFrame]->drawAngled(getX(), getY(), getScale(), angle); //draw normal image
	}
	
}

void DirectionalSprite::update(Uint32 ticks) { 
	//check if the sprite is exploding
	if ( explosion ) {
		explosion->update(ticks);
		//if the explosion is completed
		if ( explosion->chunkCount() == 0 ) {
			delete explosion;
			explosion = NULL;
		}
	}
	//do a normal update
	else {
		Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
		if (incr != Vector2f(0, 0)) {advanceFrame(ticks);}
		else {
			setCurrentFrame(0);
		}
		setPosition(getPosition() + incr);
	}
}

//create the explosion
void DirectionalSprite::explode() {
	//std::cout << "explode!!!!" << std::endl;
	if ( !explosion ) { 
		explosion = new Explosion(getName(), getPosition(), getVelocity(), getImage());
		SoundHandle::getInstance()["Explode"];
	}
}
