#include "input.h"


bool Input::keys[65535];
bool Input::prevKeys[65535];
int Input::mouse_x = 0;
int Input::mouse_y = 0;

bool Input::isKeyDown(unsigned char key) {
	if (key < 0 || key > 65535) return false;
	return keys[key];
}

bool Input::isKeyDownOnce(unsigned char key) {
	if (key < 0 || key > 65535) return false;
	if (keys[key] && !prevKeys[key]) {
		prevKeys[key] = true;
		return true;
	}

	return false;
}