#include <iostream>
#include <cmath>
#include "Explosion.h"

Explosion::Explosion(const std::string& name, const Vector2f& pos, const Vector2f& vel, const Image * image) :
	Sprite(name, pos, vel, image), 
	chunks(), 
	freeList(),
	randomness(Gamedata::getInstance().getXmlInt(name+"/Chunk/randomness")),
	minspeed(Gamedata::getInstance().getXmlInt(name+"/Chunk/minspeed"))
{
	makeChunks(Gamedata::getInstance().getXmlInt(name+"/Chunk/size"));
}

Explosion::Explosion(const Sprite& s) :
	Sprite(s), 
	chunks(), 
	freeList(),
	randomness(Gamedata::getInstance().getXmlInt(s.getName()+"/Chunk/randomness")),
	minspeed(Gamedata::getInstance().getXmlInt(s.getName()+"/Chunk/minspeed"))
{
	makeChunks(Gamedata::getInstance().getXmlInt(s.getName()+"/Chunk/size"));
}



Explosion::~Explosion() { 
	for ( Chunk* c : chunks ) {
		delete c;
	}
	for ( Chunk* c : freeList ) {
		delete c;
	}
	chunks.clear();     // clearing a vector is a waste of time, but ...
	freeList.clear();   // still ...
}

void Explosion::draw() const { 
	for ( Chunk* chunk : chunks ) {
		chunk->draw();
	}
}

void Explosion::update(Uint32 ticks) { 
	std::list<Chunk*>::iterator ptr = chunks.begin();
	while (ptr != chunks.end()) {
		(*ptr)->update(ticks); 	//update each chunk
		if ( (*ptr)->goneTooFar()) {  // Check to see if we should free a chunk
			freeList.push_back(*ptr);
			ptr = chunks.erase(ptr);
		}   
		else ++ptr;
	}
}

void Explosion::makeChunks(unsigned int n) { 
	// Break the SDL_Surface into n*n squares; where each square
	// has width and height of imageWidth/n and imageHeight/n
	// Note that "n" s/b a perfect square.
	if (n == 0) {n = 1;}
	int chunk_width = std::max(1u, getImage()->getWidth()/n);
	int chunk_height = std::max(1u, getImage()->getHeight()/n);
	int speedx = static_cast<int>(getVelocityX()); // Wanna test for zero...
	int speedy = static_cast<int>(getVelocityY()); // Make sure it's an int.
	if (speedx == 0) speedx = 1; // Make sure it's not 0;
	if (speedy == 0) speedy = 1; // Make sure it's not 0;

	const Image* proto = getImage();

	int source_y = 0;
	while  (source_y+chunk_height < getImage()->getHeight() ) {
		int source_x = 0;
		while ( source_x+chunk_width < getImage()->getWidth() ) {
			// Give each chunk it's own speed/direction:
			float sy;
			float sx = ((rand() % randomness)) * (rand()%2?-1:1); // 'cause %0 is 
			if (fabs(sx) < sin(30.0 / 180.0 * M_PI) * minspeed * 2) { //if change in x is small
			//ensure y is large
				sy = ((rand() % randomness) + randomness) * (rand()%2?-1:1);
			}
			else {sy = ((rand() % randomness)) * (rand()%2?-1:1);}


			Image* image = 
				proto->crop({source_x,source_y,chunk_width,chunk_height});
			Chunk* chunk = new Chunk(
				Vector2f(getX()+source_x,   // x coord of destination 
						getY()+source_y),  // y coord of destination
				Vector2f(sx, sy),
				getName(),
				image);
			chunk->setScale( getScale() );
			chunks.push_back(chunk);
			source_x += chunk_width;
		}
		source_y += chunk_height;
	}
}
