#include "Resource.h"

class Medkit : public Resource {
	public: 
		Medkit(int);
		virtual int gatherResource() const;
	private:
		int health;
};
