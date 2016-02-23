#include "vertexbuffer.h"
#include "graphics.h"

VertexBuffer::VertexBuffer(void* data, size_t size, size_t stride) {
	this->size = size;
	this->stride = stride;
	D3D11_BUFFER_DESC d;
	ZeroMemory(&d, sizeof(D3D11_BUFFER_DESC));

	d.Usage = D3D11_USAGE_DYNAMIC;
	d.ByteWidth = size;
	d.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	D3D11_SUBRESOURCE_DATA init;
	ZeroMemory(&init, sizeof(D3D11_SUBRESOURCE_DATA));
	init.pSysMem = data;
	
	DX::device->CreateBuffer(&d, &init, &buffer);
}

VertexBuffer::~VertexBuffer() {
	buffer->Release();
}

void VertexBuffer::bind() {
	unsigned int offset = 0;
	DX::cxt->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
}