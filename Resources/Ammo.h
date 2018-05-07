#include "Resource.h"

class Ammo : public Resource {
	public: 
		Ammo(int);
		virtual int gatherResource() const;
	private:
		int bullets; //number of bullets held in the ammo case
};
