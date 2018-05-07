#include "Resource.h"

class GodGunPart : public Resource {
	public: 
		GodGunPart(int);
		virtual int gatherResource() const;
	private:
		int numparts; //number of parts in the container
};
