#ifndef RESOURCE_H
#define RESOURCE_H
#include "../sprite.h"
#include "../Explosions/Explosion.h"

class Resource : public Sprite {
	public:
		Resource(const std::string&, const Vector2f&);
		virtual ~Resource();
		const Explosion * getExplosion() const;
		void explode();
		virtual void draw() const;
		virtual void update(Uint32);
		Resource(const Resource&) = delete;
		Resource& operator=(const Resource&);
		void setPlayerPosition(const Vector2f&);
		virtual int gatherResource() const {return 0;}
		bool isDestroyed();
	private:
		Vector2f playerPos;
		Explosion * explosion;
		bool destroyed;
};
#endif
