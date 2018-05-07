#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <sys/time.h>
#include "../DirectionalSprite.h"

class Projectile : public DirectionalSprite {
	public:
		Projectile(const std::string&);
		Projectile(const std::string&, const Vector2f&, const Vector2f&);
		float getSpeed();
		virtual void update(Uint32);
		virtual void draw() const;
		
		int getPenetrate() const; 
		void setPenetrate(int); 
		
		float getRange() const;
		void setRange(float);
		
		struct timeval getStartLife() const;
		void setStartLife(struct timeval);
		
		float getLifeSpan() const;
		void setLifeSpan(float);
		
		void reset();
		
	private:
		//Vector2f playerPos;
		float damage;		//damage of projectile (unused)
		float speed;		//speed of projectile
		float range;	//distance projectile can travel (DEFUNCT)
		float origRange;	//remaining distance projectile can travel (DEFUNCT)
		int penetrate;	//remaining number of objects that it can penetrate
		int origPen;	//number of objects that it can penetrate
		struct timeval startLife; //timestamp of projectile life
		float lifeSpan; //maximum lifespan
};
#endif
