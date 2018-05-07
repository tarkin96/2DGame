#include "Player.h"
#include "gamedata.h"
#include "Sounds/soundHandle.h"

//direction an object faces
float Player::direction(const Vector2f& player, const Vector2f& mouse) {
	return atan2(player[1] - mouse[1], player[0] - mouse[0]);
}

//normalizes the hypotenuse of a triangle (speed, distance, etc.)
Vector2f Player::normalize(float x, float y, float goal) {
		float val = pow((x * x) + (y * y), 0.5);
		float change = goal / val;
		Vector2f mod(x*change, y*change);
		return mod;
		
}

Player::Player(const std::string& str) : 
	DirectionalSprite(str),
	viewport(Viewport::getInstance()),
	aiobservers(),
	boss(),
	resources(),
	weapons(),
	currWeapon(0),
	moveSpeed(Gamedata::getInstance().getXmlFloat(str+"/moveSpeed")),
	mousex(0), mousey(0),
	screenpos(0.0, 0.0),
	HP(Gamedata::getInstance().getXmlFloat(str+"/HP")),
	colstrategies(),
	colstrategy(0),
	blink(0),
	exploding(false),
	collision(false),
	recovering(false),
	godMode(false),
	win(false),
	lose(false),
	explodingEverything(false),
	shooting(false),
	mysteryParts(0)
	
{
	//use the midpoint strategy
	colstrategies.push_back( new MidPointCollisionStrategy );
	
	//get enemy info from XML, attach AI
	boss = new Boss("Boss", getPosition(), this);
	attach(boss);
	for (int e = 0; e < Gamedata::getInstance().getXmlInt("EnemyInfo/EnemyQuantity"); e++) {
		NPC * npc = new NPC(Gamedata::getInstance().getXmlStr("EnemyInfo/Type/P" + std::to_string(e)), getPosition(), e);
		attach(npc);
		
	}
	
	//get resource info from XML, attach resources
	for (int m = 0; m < Gamedata::getInstance().getXmlInt("ResourceInfo/Medkit/MedkitQuantity"); m++) {
		Medkit * med = new Medkit(m);
		attach(med);
	}
	for (int a = 0; a < Gamedata::getInstance().getXmlInt("ResourceInfo/Ammo/AmmoQuantity"); a++) {
		Ammo * ammo = new Ammo(a);
		attach(ammo);
	}
	for (int g = 0; g < Gamedata::getInstance().getXmlInt("ResourceInfo/GodGunPart/GodGunPartQuantity"); g++) {
		GodGunPart * part = new GodGunPart(g);
		attach(part);
	}
	
	//add weapons to the player
	weapons.reserve(Gamedata::getInstance().getXmlInt("Player/WeaponCount"));
	for (int w = 0; w < Gamedata::getInstance().getXmlInt("Player/WeaponCount"); w++) {
		weapons.emplace_back(new Weapon(Gamedata::getInstance().getXmlStr("Player/Weapons/W" + std::to_string(w) + "/Name"), false));
	}
	
}

Player::Player(const Player& player) : 
	DirectionalSprite(player),
	viewport(Viewport::getInstance()),
	aiobservers(player.aiobservers),
	boss(),
	resources(player.resources),
	weapons(player.weapons),
	currWeapon(0),
	moveSpeed(player.getMoveSpeed()),
	mousex(0), mousey(0),
	screenpos(0.0, 0.0),
	HP(player.HP),
	colstrategies(),
	colstrategy(0),
	blink(0),
	exploding(false),
	collision(false),
	recovering(false),
	godMode(false),
	win(false),
	lose(false),
	explodingEverything(false),
	shooting(false),
	mysteryParts(player.mysteryParts)
	
{
	colstrategies.push_back( new MidPointCollisionStrategy );
}

Player::~Player() {
	//remove collision strats, resources, and npcs, prevent memory leaks
	for ( CollisionStrategy* strat : colstrategies ) {
		delete strat;
	}
	for ( Resource* res : resources) {
		delete res;
	}
	for ( NPC* npc : aiobservers ) {
		delete npc;
	}
	
	for (Weapon * wep : weapons) {
		delete wep;
	}
}

float Player::getMoveSpeed() const {
	return moveSpeed;
}

void Player::setMoveSpeed(float speed) {
	moveSpeed = speed;
}

const Vector2f& Player::getScreenPos() const {
	return screenpos;
}

void Player::setScreenPos(const Vector2f& pos) {
	screenpos = pos;
}

const std::list<Resource*>& Player::getResources() const {
	return resources;
}

const std::list<NPC*>& Player::getAIObservers() const {
	return aiobservers;
}

int Player::getHP() const {
	return HP;
}

int Player::getAmmo() const {
	return weapons[currWeapon]->getAmmo();
}

const std::string& Player::getWeaponName() const {
	return weapons[currWeapon]->getName();
}

void Player::update(Uint32 ticks) {
	//check if everythign is currently exploding
	if (explodingEverything) {
		for ( Resource* res : resources) {
			res->explode();
		}
		for ( NPC* npc : aiobservers ) {
			npc->explode();
			npc->setDead(true);
		}
		explodingEverything = false;
	}
	//update the base class
	collision = false;
	
	//update player's sprite if not lost or exploding
	if (!lose || getExplosion()) {
		DirectionalSprite::update(ticks);
	}
	
	//get collisions with projectiles
	findProjCollisions();
	
	//recovery time
	if (blink >= 120) {
		recovering = false;
		blink = 0;
	}

	//start recovering, find collisions, or keep recovering
	if (!recovering && getExplosion()) {recovering = true;}
	else if (!recovering && !getExplosion()) {findPlayerCollisions();}
	else {
		if (!getExplosion()) {blink++;}
	}
	
	//get mouse position to know where to face sprite
	SDL_GetMouseState(&mousex, &mousey);
	if (!lose) {
		if (shooting) {
			weapons[currWeapon]->shoot(getAngle(), Vector2f(getPosition()[0], getPosition()[1]), getVelocity(), godMode);
			shooting = false;
		}
		
		screenpos = Vector2f(getX() - Viewport::getInstance().getX() + (getScaledWidth() / 2.0), getY() - Viewport::getInstance().getY() + (getScaledHeight() / 2.0));
		setAngle(direction(Vector2f(mousex, mousey), screenpos) * 180/M_PI);
			
	}
	
	//notify all the AI and resources
	notify();
	
	//update weapons and projectiles
	for (Weapon * weapon : weapons) {
		weapon->update(ticks);
	}
	
	//chekc if an NPC is dead, detach it when it stops exploding
	std::list<NPC*>::iterator npc = aiobservers.begin();
	while(npc != aiobservers.end()) {
		if ((*npc)->isDead() && !((*npc)->getExplosion())) {
			if (dynamic_cast<Boss*>(*npc)) {
				if (!lose) {
					//victory sound
					SoundHandle::getInstance()["Airhorn"];
				}
				
			}
			npc = detach(*npc);	
		}
		
		else {
			(*npc)->update(ticks);
			npc++;
		}
	}
	//check if resource is destroyed, detach when done exploding
	std::list<Resource*>::iterator res = resources.begin();
	while(res != resources.end()) {
		if ((*res)->isDestroyed() && !((*res)->getExplosion())) {
			res = detach(*res); 
			//res++;
		}
		else {
			(*res)->update(ticks);
			res++;
		}
	}
	
	
	if (HP <= 0 && !getExplosion() && !lose) {explode(); /*HP = 100;*/ explodingEverything = true; lose = true;}
	
	if (mysteryParts == Gamedata::getInstance().getXmlInt("Weapons/TypeDefs/GodGun/PartReq")) {
		//give player the god gun
		//std::cout << "I got it!!!!!!" << std::endl;
		weapons.push_back(new Weapon("GodGun", true));
		mysteryParts = 0;
	}
	
}

void Player::draw() const {
	colstrategies[colstrategy]->draw();
	if ( collision ) {
		IOmod::getInstance().writeText("Collision!", 600, 425);
	}
	//draw the resources
	for(Resource * res : resources) {
		if (viewport.check(res)) {
			res->draw();
		}
		
	}
	
	//draw the npcs
	for(NPC * npc : aiobservers) {
		if (viewport.check(npc)) {
			//std::cout << "drawing npc " << npc << std::endl;
			npc->draw();
		}	
	}
	
	for (Weapon * weapon : weapons) {
		weapon->draw();
	}
	
	if (!lose && !getExplosion()) {
		if (recovering && !getExplosion()) {
			if (blink % 20 >= 10) {
				DirectionalSprite::draw();
			}
			else {
				
			}
		}
		else {
			//draw base class
			DirectionalSprite::draw();
		}
	}	
	

}

void Player::left() {
	if ( getX() > 0) {
		if (getVelocityY() != 0.0) {
			setVelocity(normalize(-moveSpeed, getVelocityY(), moveSpeed));
		}
		else {setVelocityX(-moveSpeed);}
		
	}
	
}

void Player::right() {
	if ( getX() < getWorldWidth()-getScaledWidth()) {
		if (getVelocityY() != 0.0) {
			setVelocity(normalize(moveSpeed, getVelocityY(), moveSpeed));
		}
		else {setVelocityX(moveSpeed);}
	}	
	
}

void Player::up() {
  if ( getY() > 0) {
    	if (getVelocityX() != 0.0) {
			setVelocity(normalize(getVelocityX(), -moveSpeed, moveSpeed));
		}
		else {setVelocityY(-moveSpeed);}
  }
}

void Player::down() {
  if ( getY() < getWorldHeight()-getScaledHeight()) {
        if (getVelocityX() != 0.0) {
			setVelocity(normalize(getVelocityX(), moveSpeed, moveSpeed));
		}
		else {setVelocityY(moveSpeed);}
  }
}

void Player::stop() {
	setVelocity(Vector2f(0.0, 0.0));
	
}

void Player::attach(NPC* npc) {
	aiobservers.push_back(npc);
}

//remove an npc from the scene
std::list<NPC*>::iterator Player::detach(NPC* npc) {
	//std::cout << "detached NPC" << std::endl;
	std::list<NPC*>::iterator ptr = aiobservers.begin();
	while (ptr != aiobservers.end()) {
		if (*ptr == npc) {
			delete * ptr;
			ptr = aiobservers.erase(ptr);
			return ptr;
		}
		else {
			ptr++;
		}
		
	}
	return ptr;
}

//add an npc to the scene
void Player::attach(Resource* res) {
	resources.push_back(res);
}

//remove a resource from the scene
std::list<Resource*>::iterator Player::detach(Resource* res) {
	//std::cout << "detached Resource" << std::endl;
	std::list<Resource*>::iterator ptr = resources.begin();
	while (ptr != resources.end()) {
		if (*ptr == res) {
			delete * ptr;
			ptr = resources.erase(ptr);
			return ptr;
		}
		else {
			ptr++;
		}
	}
	return ptr;
}

//notify all the npcs and resources of player position
void Player::notify() {
	for (NPC * npc : aiobservers) {
		//notify npcs of player position
		npc->setPlayerPosition(getPosition());
	}
	for (Resource * res : resources) {
		//notify resource of player position
		res->setPlayerPosition(getPosition());
	}
}

//SHOOTING STUFF (Project 5)!!!!!
void Player::shoot() {
	//allow shootign if player is not exploding
	if (!getExplosion()) {
		shooting = true;
	}
}

void Player::cycleWeapons() {
	currWeapon = (currWeapon + 1) % weapons.size();
}

void Player::setGodMode() {
	if (godMode) {godMode = false;}
	else {godMode = true;}
}

//COLLISION STUFF
void Player::findProjCollisions() {
	//for each weapon
	for (Weapon * weapon : weapons) {
		//check if there are active projectiles
		if (weapon->getActiveProjCount() > 0) {
			//for each projectile
			for (Projectile* proj : weapons[currWeapon]->getActiveProj()) {
				//make sure projecile exists
				if (proj) {
					//for each NPC in the scene
					for (NPC*& npc : aiobservers) {
						//make sure npc is not dead (or exploding)
						if (!(npc->isDead())) {
							//check for collision
							if (colstrategies[colstrategy]->execute(*proj, *npc)) {
								//std::cout << "collision with: " << npc->getName() << std::endl; 
								proj->setPenetrate(proj->getPenetrate() - 1);
								//check if npc is the boss
								if (dynamic_cast<Boss*>(npc)) {
									//does player have a god gun?
									if (weapon->isGod()) {
										//std::cout << "holy crap, you killed him?!" << std::endl;
										npc->explode();
										npc->setDead(true);
										explodingEverything = true;
										win = true;
										SoundHandle::getInstance()["Scream"];
									}
									//if not, nothign happens
									//else {std::cout << "life sucks, it's a boss" << std::endl;}
								}
								else {
									//destroy the npc
									//std::cout << "destroyed npc" << std::endl;
									collision = true;
									npc->explode();
									npc->setDead(true);
								}
							}
						}

					}
					std::list<Resource*>::iterator res = resources.begin();
					while(res != resources.end()) {
						if (colstrategies[colstrategy]->execute(*proj, **res)) {
							if (!(*res)->isDestroyed()) {
								proj->setPenetrate(proj->getPenetrate() - 1);
								if (dynamic_cast<Ammo*>(*res)) {
								}
								else if (dynamic_cast<Medkit*>(*res)) {
								}
								//std::cout << "destroyed resource" << std::endl;
								collision = true;
								(*res)->explode();
							}
							res++;
						}
						else {
							res++;
						}
					}		
				}
			}
		}
	}

}

void Player::findPlayerCollisions() {
	//do not check for collisions if in god mode
	if (!godMode) {
		//check for collisions with npcs
		for (const NPC* npc : aiobservers) {
			if (!npc->isDead()) {
				//when an npc hits the player
				if (colstrategies[colstrategy]->execute(*this, *npc)) {
					if (!recovering) {
						collision = true;
						startRecovering();
						HP-=10;
						SoundHandle::getInstance()["Scream"];
					}

				}
			}

		}
	}

	//check for collisions with resources
	std::list<Resource*>::iterator res = resources.begin();
	while(res != resources.end()) {
		if (!((*res)->isDestroyed())) {
			if (colstrategies[colstrategy]->execute(*this, **res)) {
				collision = true;
				//pick up ammo
				if (dynamic_cast<Ammo*>(*res)) {
					if (!weapons[currWeapon]->isGod()) {
						weapons[currWeapon]->setAmmo(weapons[currWeapon]->getAmmo() + (*res)->gatherResource());
						res = detach(*res);
						SoundHandle::getInstance()["Pickup"];
					}
					res++;
				}
				//pick up health
				else if (dynamic_cast<Medkit*>(*res)) {
					HP += (*res)->gatherResource();
					res = detach(*res);
					SoundHandle::getInstance()["Pickup"];
				}
				//pick up parts for god gun
				else if (dynamic_cast<GodGunPart*>(*res)) {
					mysteryParts++;
					res = detach(*res);
				}
			}
			else {
				res++;
			}
		}
		else {res++;}
	}	
}

//RECOVERING FROM THE EXPLOSION
void Player::startRecovering() {
	recovering = true;
}
