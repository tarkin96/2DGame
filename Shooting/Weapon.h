#include "Projectile.h"
class Weapon {
	public:
		Weapon(const std::string&, bool);
		~Weapon();
		void shoot(float, const Vector2f&, const Vector2f&, bool);
		int getAmmo() {return ammo;}
		void setAmmo(int a) {ammo = a;}
		const std::vector<Projectile>& getProjectiles() const;
		const std::vector<Projectile*>& getActiveProj() const;
		int getActiveProjCount() const {return activeProjCount;}
		const std::string& getName() const;
		
		void update(Uint32);
		void draw() const;
		
		bool isGod() {return god;}
	private:
		//Projectile * proj;
		std::vector<Projectile*> activeProj;
		std::vector<Projectile> projectiles;
		unsigned long int currActiveProj;
		unsigned long int currProj;
		unsigned int activeProjCount;
		
		//void findProjCollisions();
		//void findPlayerCollisions();
		
		int ammo;
		int arcCount;
		int arcSize;
		std::string nameType;
		std::string sound;
		bool god;
};
