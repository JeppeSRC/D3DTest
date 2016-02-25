#pragma once
#include "IndexBuffer.h"
#include "vertexbuffer.h"

struct Model {
	VertexBuffer* vertex;
	IndexBuffer* index;

	Model() {
		vertex = nullptr;
		index = nullptr;
	}

	~Model() {
		delete vertex, index;
	}
};

void loadObjModel(const char* filename, Model* model);