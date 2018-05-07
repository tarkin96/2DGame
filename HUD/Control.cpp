#include "Control.h"


Control::Control(const std::string& act, const std::string& but, const Vector3f& col, const Vector2f& pos, int size) :
action(act), button(but), color(col), position(pos), fontsize(size) {}

std::string Control::getAction() const {
	return action;
}
void Control::setAction(const std::string& str) {
	action = str;
}
std::string Control::getButton() const {
	return button;
}
void Control::setButton(const std::string& str) {
	button = str;
}
Vector3f Control::getColor() const {
	return color;
}
void Control::setColor(const Vector3f& vec) {
	color = vec;
}
Vector2f Control::getPosition() const {
	return position;
}
void Control::setPosition(const Vector2f& vec) {
	position = vec;
}
int Control::getFontSize() const {
	return fontsize;
}
void Control::setFontSize(int size) {
	fontsize = size;
}
