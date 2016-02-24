#pragma once

class Input {
public:
	static bool keys[];
	static bool prevKeys[];

	static int mouse_x;
	static int mouse_y;

	static bool isKeyDown(unsigned char key);
	static bool isKeyDownOnce(unsigned char key);
};
