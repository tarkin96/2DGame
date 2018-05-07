#ifndef HUD_H
#define HUD_H
#include <string>
#include <sstream>
#include <vector>
#include "Control.h"
#include "../renderContext.h"
#include "../ioMod.h"
#include "../clock.h"
#include "../image.h"
#include "../Player.h"

class HUD {
	public:
		HUD();
		~HUD();
		void draw() const;
		void update();
		void attachPlayer(const Player*);	//tells HUD to track a certain player object
		
	private:
		Vector3f fps_color, name_color, hp_color, ammo_color, weapon_color; //tracks color of HUD text
		Vector2f fps_position, name_position, hp_position, ammo_position, weapon_position; //tracks position of HUD text
		int fps_size, name_size, hp_size, ammo_size, weapon_size; //tracks font size of HUD text
		//std::string name;
		
		
		std::string dev_name, hp_text, ammo_text, weapon_text; //tracks initial text of HUD
		//std::string controlstext;
		IOmod& iomod;
		Clock& clock;
		std::vector<Image*> images;
		std::vector<Control> controls;

		const Player * player; //player tracked by the hUD
		int health;
		int ammo;
		std::string weapon;
		HUD(const HUD&) = delete;
		HUD& operator=(const HUD&);
		std::vector<SDL_Texture*> textures;
		//SDL_Texture controlHeader;
		//int transparency;
		//std::map<std::stringname
};
#endif
