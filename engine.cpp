#include <iostream>
#include <algorithm>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "Multisprite.h"
#include "DirectionalSprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"

Engine::~Engine() { 
	for ( CollisionStrategy* strat : strategies ) {
		delete strat;
	}
	strategies.clear();
	delete player;
	std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
	rc( RenderContext::getInstance() ),
	io( IOmod::getInstance() ),
	soundhandler(SoundHandle::getInstance()),
	clock( Clock::getInstance() ),
	renderer( rc->getRenderer() ),
	hud(),
	world("back", Gamedata::getInstance().getXmlFloat("back/factor") ),
	flames("flames",  Gamedata::getInstance().getXmlFloat("flames/factor") ),
	viewport( Viewport::getInstance() ),
	strategies(),
	strategy(0),
	collision(false),
	player(new Player("Player")),
	currentSprite(0),
	showHUD(true), //make sure HUD is visible at start
	makeVideo( false )
{
	//add midpoint collision
	strategies.push_back( new MidPointCollisionStrategy );
	Viewport::getInstance().setObjectToTrack(player);
	hud.attachPlayer(player);
	std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {


	world.draw();
	flames.draw();


	player->draw();
	if (showHUD) {
		hud.draw();
	}
	strategies[strategy]->draw();
	if ( collision ) {
		IOmod::getInstance().writeText("Collision!", 600, 425);
	}
	
	if (player->getExplosion() || player->getLose()) {
		io.writeText("You have succumbed to the nightmare! Please Restart!", 100, 210, {0, 255, 0, 255}, 30);
	}
	else if (player->getWin()) {
		io.writeText("In Helmo's World, No one Wins! Please Restart!", 100, 210, {0, 255, 0, 255}, 30);
		//return;
	}

	viewport.draw();
	SDL_RenderPresent(renderer);
}

void Engine::findCollisions() {

}

void Engine::update(Uint32 ticks) {

	findCollisions();
	player->update(ticks);
	world.update();
	flames.update();
	hud.update();
	player->stop();
	viewport.update(); // always update viewport last

}

//this is no longer implemented, kept for future reference
void Engine::switchSprite(){
  ++currentSprite;
}

void Engine::play() {
	SDL_Event event;
	const Uint8* keystate;
	bool done = false;
	Uint32 ticks = clock.getElapsedTicks();
	FrameGenerator frameGen;

	while ( !done ) {
		//makes HUD disappear after 5 seconds (removed for final project)
		//if (clock.getSeconds() == 5) {showHUD = false;}
		// The next loop polls for events, guarding against key bounce:
		while ( SDL_PollEvent(&event) ) {
			keystate = SDL_GetKeyboardState(NULL);
			
			if (event.type ==  SDL_QUIT) { done = true; break; }
			if(event.type == SDL_KEYDOWN) {
				if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
					done = true;
					break;
				}
				if ( keystate[SDL_SCANCODE_P] ) {
					if ( clock.isPaused() ) clock.unpause();
					else clock.pause();
				}

				if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
					std::cout << "Initiating frame capture" << std::endl;
					makeVideo = true;
				}
				else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
					std::cout << "Terminating frame capture" << std::endl;
					makeVideo = false;
				}
				if(keystate[SDL_SCANCODE_F1]) {
					if (showHUD) {showHUD=false;}
					else {showHUD=true;}
				}
				if(keystate[SDL_SCANCODE_E]) {
					player->explode(); //suicide the player
				}
				if(keystate[SDL_SCANCODE_R]) {
					Restart(); //restart the game
				}
				if ( keystate[SDL_SCANCODE_SPACE] ) {
					player->setGodMode(); //toggle god mode
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					player->shoot(); //shoot
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) {
					player->cycleWeapons(); //switch weapons
				}
			}	
		}

		// In this section of the event loop we allow key bounce:
		ticks = clock.getElapsedTicks();
		if ( ticks > 0 ) {
			clock.incrFrame();
			if (keystate[SDL_SCANCODE_A]) {
				player->left();
			}
			if (keystate[SDL_SCANCODE_D]) {
				player->right();
			}
			if (keystate[SDL_SCANCODE_W]) {
				player->up();
			}
			if (keystate[SDL_SCANCODE_S]) {
				player->down();
			}
			update(ticks);
			draw();
			if ( makeVideo ) {
				frameGen.makeFrame();
			}
		}
	}
}

void Engine::Restart() {
	delete player; //delete player
	
	std::cout << "Restarting program" << std::endl;
	
	player = new Player("Player"); //create new player (which re-reads everythign from xml again)
	//start trackign new player
	Viewport::getInstance().setObjectToTrack(player);
	hud.attachPlayer(player);
	
}
