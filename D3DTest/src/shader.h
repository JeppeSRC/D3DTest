#pragma once
#include "graphics.h"
#include <d3dcompiler.h>

class Shader {
private:
	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;

	struct {

		unsigned char* byteCode;
		size_t size;
	} vCode, pCode;



	void Init(void* v, size_t vsize, void* p, size_t psize);

public:
	Shader(const char* vertex, const char* pixel);
	Shader(void* vertex, size_t vsize, void* pixel, size_t psize);
	~Shader();

	void bind();

	void VSPassBuffers(unsigned int buffer_id, size_t size, void* data);
	void PSPassBuffers(unsigned int buffer_id, size_t size, void* data);

	inline const ID3D11VertexShader* getVShader() const { return vs; }
	inline const ID3D11PixelShader*  getPShader() const { return ps; }

	inline const void* getVCode() const { return vCode.byteCode; }
	inline const void* getPCode() const { return pCode.byteCode; }
	inline size_t getVCodeSize() const { return vCode.size; }
	inline size_t getPCodeSize() const { return pCode.size; }
};
