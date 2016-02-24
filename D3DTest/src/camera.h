#pragma once
#include "graphics.h"
#include "../include/math/math.h"

class Camera {
protected:
	tomato::math::vec3 position;
	tomato::math::vec3 rotation;
	tomato::math::mat4 viewMatrix;

public:
	Camera(const tomato::math::vec3& pos, const tomato::math::vec3& rot) {
		position = pos;
		rotation = rot;
		viewMatrix = tomato::math::mat4(1);
	}

	virtual void update(float delta) { }

	inline const tomato::math::mat4& getViewMatrix() const { return viewMatrix; }
};