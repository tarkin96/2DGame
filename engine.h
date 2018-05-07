#include <vector>
#include <SDL.h>
//#include "ioMod.h"
#include "renderContext.h"
//#include "clock.h"
#include "world.h"
//#include "viewport.h"
#include "Player.h"
#include "HUD/HUD.h"
//#include "collisionStrategy.h"
#include "Sounds/soundHandle.h"

class Engine {
	public:
		Engine ();
		~Engine ();
		void play();
		void switchSprite();
		void Restart();

	private:
		const RenderContext* rc;
		const IOmod& io;
		const SoundHandle& soundhandler;
		Clock& clock;

		SDL_Renderer * const renderer;
		HUD hud;
		World world;
		World flames;
		Viewport& viewport;

		//collision strategies
		std::vector<CollisionStrategy*> strategies;
		int strategy;
		void findCollisions();
		bool collision;

		//test sprites
		Drawable* Bigbird;
		Drawable* Fireball;

		Player * player; 
		int currentSprite;

		//made this for HUD
		bool showHUD;

		bool makeVideo;

		void draw() const;
		void update(Uint32);

		//prevent compiler from defining
		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;
		void printScales() const;

};
