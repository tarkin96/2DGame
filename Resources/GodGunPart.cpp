#include "GodGunPart.h" 

GodGunPart::GodGunPart(int num) : 
	Resource("GodGunPart", Vector2f(Gamedata::getInstance().getXmlFloat("ResourceInfo/GodGunPart/GodGunPart" + std::to_string(num) + "/x"), 
		Gamedata::getInstance().getXmlFloat("ResourceInfo/GodGunPart/GodGunPart" + std::to_string(num) + "/y"))),
		numparts(0)
{
}

int GodGunPart::gatherResource() const {
	return numparts; //god gun parts in the case
}
