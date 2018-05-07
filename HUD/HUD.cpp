#include "HUD.h"

HUD::HUD() : 
	fps_color(Vector3f(Gamedata::getInstance().getXmlInt("HUD/fps/color/r"),
		Gamedata::getInstance().getXmlInt("HUD/fps/color/g"),
		Gamedata::getInstance().getXmlInt("HUD/fps/color/b"))),
	name_color(Vector3f(Gamedata::getInstance().getXmlInt("HUD/name/color/r"),
		Gamedata::getInstance().getXmlInt("HUD/name/color/g"),
		Gamedata::getInstance().getXmlInt("HUD/name/color/b"))),
	hp_color(Vector3f(Gamedata::getInstance().getXmlInt("HUD/HP/color/r"),
		Gamedata::getInstance().getXmlInt("HUD/HP/color/g"),
		Gamedata::getInstance().getXmlInt("HUD/HP/color/b"))),
	ammo_color(Vector3f(Gamedata::getInstance().getXmlInt("HUD/Ammo/color/r"),
		Gamedata::getInstance().getXmlInt("HUD/Ammo/color/g"),
		Gamedata::getInstance().getXmlInt("HUD/Ammo/color/b"))),
	weapon_color(Vector3f(Gamedata::getInstance().getXmlInt("HUD/Weapon/color/r"),
		Gamedata::getInstance().getXmlInt("HUD/Weapon/color/g"),
		Gamedata::getInstance().getXmlInt("HUD/Weapon/color/b"))),
	fps_position(Vector2f(Gamedata::getInstance().getXmlInt("HUD/fps/position/x"),
		Gamedata::getInstance().getXmlInt("HUD/fps/position/y"))),
	name_position(Vector2f(Gamedata::getInstance().getXmlInt("HUD/name/position/x"),
		Gamedata::getInstance().getXmlInt("HUD/name/position/y"))),
	hp_position(Vector2f(Gamedata::getInstance().getXmlInt("HUD/HP/position/x"),
		Gamedata::getInstance().getXmlInt("HUD/HP/position/y"))),
	ammo_position(Vector2f(Gamedata::getInstance().getXmlInt("HUD/Ammo/position/x"),
		Gamedata::getInstance().getXmlInt("HUD/Ammo/position/y"))),
	weapon_position(Vector2f(Gamedata::getInstance().getXmlInt("HUD/Weapon/position/x"),
		Gamedata::getInstance().getXmlInt("HUD/Weapon/position/y"))),
	fps_size(Gamedata::getInstance().getXmlInt("HUD/fps/fontsize")),
	name_size(Gamedata::getInstance().getXmlInt("HUD/name/fontsize")),
	hp_size(Gamedata::getInstance().getXmlInt("HUD/HP/fontsize")),
	ammo_size(Gamedata::getInstance().getXmlInt("HUD/Ammo/fontsize")),
	weapon_size(Gamedata::getInstance().getXmlInt("HUD/Weapon/fontsize")),
	//filename(Gamedata::getInstance().getXmlStr("HUD/file")),
	dev_name(Gamedata::getInstance().getXmlStr("HUD/name/text")),
	hp_text(Gamedata::getInstance().getXmlStr("HUD/HP/text")),
	ammo_text(Gamedata::getInstance().getXmlStr("HUD/Ammo/text")),
	weapon_text(Gamedata::getInstance().getXmlStr("HUD/Weapon/text")),
	//controlstext(Gamedata::getInstance().getXmlStr("HUD/controls/text/text")),
	iomod( IOmod::getInstance() ),
	clock( Clock::getInstance() ),
	images(),
	controls(),
	player(),
	health(Gamedata::getInstance().getXmlInt("Player/HP")),
	ammo(Gamedata::getInstance().getXmlInt("Player/StartAmmo")),
	weapon(),
	textures()
	
	//transparency(Gamedata::getInstance().getXmlInt("HUD/transparency"))
{
	//ENABLE IMAGES ON HUD WHEN YOU GET A CHANCE
	for(int i = 0; i < Gamedata::getInstance().getXmlInt("HUD/files/FileQuantity"); i++) {
		images.push_back(RenderContext::getInstance()->getImage(Gamedata::getInstance().getXmlStr("HUD/files/F" + std::to_string(i) + "/file")));
	}
	//GETTING CONTROLS INFO
	//loop through XML file for control info
	for(int c = 0; c < Gamedata::getInstance().getXmlInt("HUD/controls/ControlQuantity"); c++) {
		Control con(Gamedata::getInstance().getXmlStr("HUD/controls/C" + std::to_string(c) + "/Action"), 
			Gamedata::getInstance().getXmlStr("HUD/controls/C" + std::to_string(c) + "/Button"),
			Vector3f(Gamedata::getInstance().getXmlInt("HUD/controls/C" + std::to_string(c) + "/Color/r"), 
				Gamedata::getInstance().getXmlInt("HUD/controls/C" + std::to_string(c) + "/Color/g"),
				Gamedata::getInstance().getXmlInt("HUD/controls/C" + std::to_string(c) + "/Color/b")),
			Vector2f(Gamedata::getInstance().getXmlInt("HUD/controls/C" + std::to_string(c) + "/Position/x"), 
				Gamedata::getInstance().getXmlInt("HUD/controls/C" + std::to_string(c) + "/Position/y")),
			Gamedata::getInstance().getXmlInt("HUD/controls/C" + std::to_string(c) + "/Fontsize"));
		controls.push_back(con);
		//create static textures for text
		textures.push_back(iomod.CreateText(con.getAction() + ": " + con.getButton(), con.getPosition()[0], con.getPosition()[1], {(uint8_t)con.getColor()[0], (uint8_t)con.getColor()[1], (uint8_t)con.getColor()[2], 255}, con.getFontSize()));
	}
	//controlHeader(*(iomod.CreateText("Controls: ", controls[0].getPosition()[0] - 20, controls[0].getPosition()[1] - controls[0].getFontSize() - 4, {0, 0, 0, 255}, controls[0].getFontSize() + 4)));
	
}
	
HUD::~HUD() {
	for ( Image * i :images ) {
		delete i;
	}
	for (SDL_Texture * tex : textures) {
		if (!tex) {
			SDL_DestroyTexture(tex);
		}
		
	}
}

void HUD::update() {
	//get player info
	health = player->getHP();
	ammo = player->getAmmo();
	weapon = player->getWeaponName();
}

void HUD::attachPlayer(const Player* play) {
	player = play;
}

void HUD::draw() const {
	std::stringstream fps;
	fps << "FPS: " << clock.getFps();
	if (controls.size() > 0) {
		iomod.writeText("Controls: ", controls[0].getPosition()[0] - 20, controls[0].getPosition()[1] - controls[0].getFontSize() - 4, {0, 255, 0, 255}, controls[0].getFontSize() + 4);
	}
	
	int i = 0;
	//draw static text
	for (auto& tex : textures) {
		iomod.writeText(tex, controls[i].getPosition()[0], controls[i].getPosition()[1]);
		i++;
	}

	//draw dynamic text
	iomod.writeText(hp_text + std::to_string(health), hp_position[0], hp_position[1], {(uint8_t)hp_color[0], (uint8_t)hp_color[1], (uint8_t)hp_color[2], 255}, hp_size);
	iomod.writeText(weapon_text + weapon, weapon_position[0], weapon_position[1], {(uint8_t)weapon_color[0], (uint8_t)weapon_color[1], (uint8_t)weapon_color[2], 255}, weapon_size);
	iomod.writeText(ammo_text + std::to_string(ammo), ammo_position[0], ammo_position[1], {(uint8_t)ammo_color[0], (uint8_t)ammo_color[1], (uint8_t)ammo_color[2], 255}, ammo_size);
	iomod.writeText("Name: " + dev_name, name_position[0], name_position[1], {(uint8_t)name_color[0], (uint8_t)name_color[1], (uint8_t)name_color[2], 255}, name_size);
	
	


}
