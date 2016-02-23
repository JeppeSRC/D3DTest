#pragma once
#include <d3d11.h>
#include <d3d10.h>
#include <dxgi.h>


class IndexBuffer {
private:
	ID3D11Buffer* buffer;
	size_t size;
	size_t count;
	DXGI_FORMAT format;

public:
	IndexBuffer(void* data, size_t size, DXGI_FORMAT format, size_t count);
	~IndexBuffer();

	void bind();

	inline ID3D11Buffer* getBuffer() { return buffer; }
	inline size_t getCount() const { return count; }
};