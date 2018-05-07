#include "Medkit.h"
#include "../gamedata.h"

Medkit::Medkit(int num) : 
	Resource("Medkit", Vector2f(Gamedata::getInstance().getXmlFloat("ResourceInfo/Medkit/Medkit" + std::to_string(num) + "/x"), 
		Gamedata::getInstance().getXmlFloat("ResourceInfo/Medkit/Medkit" + std::to_string(num) + "/y"))),
	health(Gamedata::getInstance().getXmlInt("ResourceInfo/Medkit/Medkit" + std::to_string(num) + "/Health"))
{
}

int Medkit::gatherResource() const {
	return health; // health in the case
}
