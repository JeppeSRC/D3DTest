#pragma once
#include "camera.h"

class FlyCamera : public Camera {
public:
	FlyCamera(const tomato::math::vec3& pos, const tomato::math::vec3& rot);

	void update(float delta) override;
};
