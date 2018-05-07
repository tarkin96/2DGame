#include "Ammo.h"
#include "../gamedata.h"

Ammo::Ammo(int num) : 
	Resource("Ammo", Vector2f(Gamedata::getInstance().getXmlFloat("ResourceInfo/Ammo/Ammo" + std::to_string(num) + "/x"), 
		Gamedata::getInstance().getXmlFloat("ResourceInfo/Ammo/Ammo" + std::to_string(num) + "/y"))),
	bullets(Gamedata::getInstance().getXmlInt("ResourceInfo/Ammo/Ammo" + std::to_string(num) + "/Bullets"))
{
}

int Ammo::gatherResource() const {
	return bullets;
}
