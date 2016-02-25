#pragma once
#include "../include/math/math.h"
#include "objmodel.h"
#include "shader.h"

class Entity {
protected:
	tomato::math::vec3 position;
	tomato::math::vec3 rotation;
	Model* model;
	bool delModel;

public:
	Entity(const tomato::math::vec3& position, const tomato::math::vec3& rotation, Model* model, bool delModel = false) : position(position), rotation(rotation), delModel(delModel) { this->model = model; }
	~Entity() { if (delModel) delete model; }

	void render(Shader* shader) {
		model->index->bind();
		model->vertex->bind();

		shader->VSPassBuffers(4, sizeof(tomato::math::mat4), &(tomato::math::mat4().translate(position) * tomato::math::mat4().rotate(rotation)));
		
		DX::cxt->DrawIndexed(model->index->getCount(), 0, 0);
	}
};
