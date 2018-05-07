#ifndef PLAYER_H
#define PLAYER_H
#include <list>
#include "Multisprite.h"
#include "AI/Boss.h"
#include "viewport.h"
#include "Resources/Medkit.h"
#include "Resources/Ammo.h"
#include "Resources/GodGunPart.h"
#include "Shooting/Weapon.h"
#include "collisionStrategy.h"
#include "ioMod.h"

class Player : public DirectionalSprite {
	public:
		Player(const std::string&);
		Player(const Player&);
		~Player();
		Player& operator=(const Player&);
		
		//getters and setters
		float getMoveSpeed() const;
		void setMoveSpeed(float);
		const Vector2f& getScreenPos() const;
		void setScreenPos(const Vector2f&);
		int getHP() const;
		int getAmmo() const;
		const std::string& getWeaponName() const;
		bool getWin() const {return win;}
		bool getLose() const {return lose;}
		
		//math stuff
		Vector2f normalize(float, float, float);
		float direction(const Vector2f&, const Vector2f&);
		float getSides(float, float);
		
		const std::list<Resource*>& getResources() const;	//NPCs in the scene
		const std::list<NPC*>& getAIObservers() const;		//resources in the scene

		
		virtual void update(Uint32);
		virtual void draw() const;
		
		//control functions
		void left();
		void right();
		void up();
		void down();
		void slow();
		void stop();
		
		//observer pattern for AI and Resource stuff
		void attach(NPC *);
		std::list<NPC*>::iterator detach(NPC *);
		void attach(Resource*);
		std::list<Resource*>::iterator detach(Resource*);
		void notify();
		
		//shooting stuff
		void shoot();
		void cycleWeapons();
		void setGodMode();
	private:
		Viewport& viewport;
		std::list<NPC*> aiobservers;
		NPC * boss;
		//resource pooling
		std::list<Resource*> resources;
		
		//for Project 5 shooting
		std::vector<Weapon*> weapons;
		int currWeapon;
		
		float moveSpeed;
		int mousex, mousey;
		//position on the screen
		Vector2f screenpos;
		//current health
		int HP;
			
		//COLLISION STUFF
		std::vector<CollisionStrategy*> colstrategies;
		int colstrategy;
		int blink;
		
		void findProjCollisions();
		void findPlayerCollisions();

		void startRecovering();
		bool exploding;
		bool collision;
		bool recovering;
		bool godMode;
		bool win;
		bool lose;
		bool explodingEverything;
		bool shooting;
		int mysteryParts;
};
#endif
