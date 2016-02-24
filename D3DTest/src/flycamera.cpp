#include "FlyCamera.h"

#define SPEED 10
#define SENS 6.5

using namespace tomato::math;

FlyCamera::FlyCamera(const vec3& pos, const vec3& rot) : Camera(pos, rot) { }

void FlyCamera::update(float delta) {
	if (DX::mouseCaptured) {
		float rad = TO_RADIANS_F(rotation.y);
		if (Input::keys['W']) {
			position.z -= cosf(rad) * SPEED * delta;
			position.x -= sinf(rad) * SPEED * delta;
		} else if (Input::keys['S']) {
			position.z += cosf(rad) * SPEED * delta;
			position.x += sinf(rad) * SPEED * delta;
		}

		if (Input::keys['A']) {
			position.x -= cosf(rad) * SPEED * delta;
			position.z += sinf(rad) * SPEED * delta;
		} else if (Input::keys['D']) {
			position.x += cosf(rad) * SPEED * delta;
			position.z -= sinf(rad) * SPEED * delta;
		}

		if (Input::keys[VK_SHIFT]) {
			position.y -= SPEED * delta;
		} else if (Input::keys[VK_SPACE]) {
			position.y += SPEED * delta;
		}

		rotation.y -= float(Input::mouse_x - (DX::width >> 1)) * delta * SENS;
		rotation.x += float(Input::mouse_y - (DX::height >> 1)) * delta * SENS;



		viewMatrix = mat4().rotate(rotation) * mat4().translate(position * -1.0f);
	}


	if (Input::isKeyDownOnce(VK_TAB)){
		if (DX::mouseCaptured) DX::SetMouseState(false);
		else DX::SetMouseState(true);
	}

}