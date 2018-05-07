/* This class is used to hold information about the controls for the HUD.
 * Makes printing to the screen in IOmod easier */

#include "../gamedata.h"

class Control {
	public: 
		Control(const std::string&, const std::string&, const Vector3f&, const Vector2f&, int);
		std::string getAction() const;
		void setAction(const std::string&); 
		std::string getButton() const;
		void setButton(const std::string&); 
		Vector3f getColor() const;
		void setColor(const Vector3f&);
		Vector2f getPosition() const;
		void setPosition(const Vector2f&);
		int getFontSize() const;
		void setFontSize(int);
	private:
		std::string action;
		std::string button;
		Vector3f color;
		Vector2f position;
		int fontsize;
};

