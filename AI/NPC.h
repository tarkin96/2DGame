#ifndef NPC_H
#define NPC_H

#include "../DirectionalSprite.h"

class NPC : public DirectionalSprite {
	public:
		NPC(const std::string&, const Vector2f&, const Vector2f&, float, int, int, int, int);
		NPC(const std::string&, const Vector2f&, int);
		NPC(const std::string& name, const Vector2f& player);
		void setPlayerPosition(const Vector2f&);
		Vector2f getPlayerPosition() const;
		
		//npc tactices
		void guard();
		void chase();
		void attack();
		void search();
		void fallback();
		
		float dist(const Vector2f&, const Vector2f&);

		float dir(const Vector2f&, const Vector2f&);

		float calculateAngle(float, float);
		virtual void update(Uint32);
		virtual void draw() const;
		
		bool isDead() const;
		
		void setDead(bool);
		
	private:
		Vector2f playerPosition;
		
		Vector2f initPosition;	//guard position
		float initAngle;		//guard angle
		int playWidth, playHeight;
		int sightRange;
		int attackRange;
		enum ACTION {GUARD, CHASE, ATTACK, SEARCH, FALLBACK}; //enums for tactics
		ACTION currentAction;
		int moveSpeed;
		
		//variables for animations
		int anim_timer;
		bool playing_anim;
		bool check_anim() const;
		void start_anim();
		void update_anim();
		
		//movement
		void goLeft();
		void goRight();
		void goUp();
		void goDown();
		
		bool dead;
};
#endif
