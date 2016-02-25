#include "objmodel.h"
#include "../include/math/math.h"
#include "../include/util/string.h"
#include <vector>
#include <stdio.h>

using namespace tomato::util;

#define DELETE(strings, num) for (int i = 0; i < num; i++) { delete[] strings[i]; } \
	delete[] strings

static String* split(const char* source, size_t& numLines, char delimiter) {


	size_t length = strlen(source);
	unsigned int numSplits = 0;
	for (size_t i = 0; i < length; i++) {
		if (source[i] == delimiter) numSplits++;
	}

	String* lines = new String[numSplits+1];
	numSplits = 0;
	size_t lastIndex = 0;

	for (size_t i = 0; i < length; i++) {
		if (source[i] == delimiter) {
			size_t newlen = i - lastIndex;
			lines[numSplits] = String(source + lastIndex, newlen);
		/*	lines[numSplits] = new char[newlen + 1];
			memset(lines[numSplits], 0, newlen + 1);
			memcpy(lines[numSplits], source + lastIndex, newlen);*/

			lastIndex = i + 1;
			numSplits++;
		}
	}


	if (length - lastIndex > 0) {
		size_t newlen = length - lastIndex;
		lines[numSplits] = String(source + lastIndex, newlen);
	/*	lines[numSplits] = new char[newlen + 1];
		memset(lines[numSplits], 0, newlen + 1);
		memcpy(lines[numSplits], source + lastIndex, newlen);*/
		numSplits++;
	}

	numLines = numSplits;

	return lines;

}
using namespace tomato::math;

static void InsertFace(char* face, std::vector<vec3>& vertices, std::vector<vec3>& norm, std::vector<vec2>& tex, std::vector<vec3>& normals, std::vector<vec2>& uv, std::vector<unsigned int>& indices) {
	size_t values = 0;
	String* v = split(face, values, '/');

	unsigned int vertex = atoi(v[0].str) - 1;

	indices.push_back(vertex);

	uv[vertex] = tex[atoi(v[1].str) - 1];
	normals[vertex] = norm[atoi(v[2].str) - 1];

	delete[] v;
}

void loadObjModel(const char* filename, Model* model) {


	FILE* file = fopen(filename, "rt");

	if (file) {

		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		fseek(file, 0, SEEK_SET);

		size_t numLines = 0;
		String* lines;

		char* data = new char[size + 1];
		memset(data, 0, size + 1);

		fread(data, size, 1, file);

		fclose(file);

		lines = split(data, numLines, '\n');

		delete[] data;


		std::vector<vec3> vertices, normals, norm;
		std::vector<vec2> uv, tex;
		std::vector<unsigned int> indices;
		size_t lastIndex = 0;
		bool initialized = false;
		for (size_t i = 0; i < numLines; i++) {
			const char* line = lines[i].str;

			if (line[0] == 'v' && line[1] == ' ') {
				vec3 tmp;
				sscanf(line, "v %f %f %f", &tmp.x, &tmp.y, &tmp.z);
				vertices.push_back(tmp);
			} else if (line[0] == 'v' && line[1] == 'n') {
				vec3 tmp;
				sscanf(line, "vn %f %f %f", &tmp.x, &tmp.y, &tmp.z);
				norm.push_back(tmp);
			} else if (line[0] == 'v' && line[1] == 't') {
				vec2 tmp;
				sscanf(line, "vt %f %f", &tmp.x, &tmp.y);
				tex.push_back(tmp);
			} else if (line[0] == 'f' && line[1] == ' ') {
				lastIndex = i;
				normals.resize(vertices.size());
				uv.resize(vertices.size());
				break;
			} else {
				continue;
			}
		}



		for (size_t i = lastIndex; i < numLines; i++) {
			const char* line = lines[i].str;

			size_t numFaces = 0;
			String* faces = split(line, numFaces, ' ');

			for (size_t j = 1; j < numFaces; j++)
				InsertFace(faces[j].str, vertices, norm, tex, normals, uv, indices);

			delete[] faces;
		}

		delete[] lines;

		struct Vertex {
			vec3 position; //float x, y, z;
			vec4 color;//float r, g, b, a;
			vec2 uv;//float u, v;
			vec3 normal;
		};

		Vertex* v = new Vertex[vertices.size()];
		size_t vsize = vertices.size();
		for (size_t i = 0; i < vsize;i++) {
			v[i].position = vertices[i];
			v[i].color = vec4(1, 0, 1, 1);
			v[i].uv = uv[i];
			v[i].normal = normals[i];
		}

		if (model == nullptr) {
			goto end;
		}

		model->vertex = new VertexBuffer(v, vsize * sizeof(Vertex), sizeof(Vertex));
		model->index= new IndexBuffer(&indices[0], indices.size() * sizeof(unsigned int), DXGI_FORMAT_R32_UINT, indices.size());
		
		end:

		delete[] v;


	} else {
		//TODO: error
	}

}