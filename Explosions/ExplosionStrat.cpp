#include "ExplosionStrat.h"
/* This is not used!!!!!!*/

//FOR CHUNK EXPLOSIONS
bool ChunkExplosionStrat::execute(const Drawable&, const Drawable&) const {
	
	return true;
}
void ChunkExplosionStrat::draw() const {
	
}

//FOR ANIMATED EXPLOSIONS
bool AnimationExplosionStrat::execute(const Drawable&, const Drawable&) const {
	return true;
}
void AnimationExplosionStrat::draw() const {
	
}
