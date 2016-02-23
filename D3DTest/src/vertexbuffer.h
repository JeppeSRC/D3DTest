#pragma once
#include <d3d11.h>
#include <d3d10.h>
#include <dxgi.h>


class VertexBuffer {
private:
	ID3D11Buffer* buffer;
	unsigned int size;
	unsigned int stride;

public:
	VertexBuffer(void* data, size_t size, size_t stride);
	~VertexBuffer();

	void bind();

	inline ID3D11Buffer* getBuffer() { return buffer; }
};