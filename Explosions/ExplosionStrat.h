/* This is not used. */

#ifndef EXPSTRAT_H
#define EXPSTRAT_H

#include <iostream>
#include "../sprite.h"
#include "../gamedata.h"
#include "chunk.h"

class ExplosionStrat {
	public:
	virtual bool execute(const Drawable&, const Drawable&) const = 0;
	virtual void draw() const = 0;
	virtual ~ExplosionStrat() {}
};

class ChunkExplosionStrat : public ExplosionStrat {
public:
	ChunkExplosionStrat() {}
	virtual bool execute(const Drawable&, const Drawable&) const;
	virtual void draw() const;
};

class AnimationExplosionStrat : public ExplosionStrat {
public:
	AnimationExplosionStrat() {}
	virtual bool execute(const Drawable&, const Drawable&) const;
	virtual void draw() const;
	//float distance(float, float, float, float) const;
};

#endif
