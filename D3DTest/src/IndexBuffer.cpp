#include "IndexBuffer.h"
#include "graphics.h"

IndexBuffer::IndexBuffer(void* data, size_t size, DXGI_FORMAT format, size_t count) {
	this->size = size;
	this->format = format;
	this->count = count;
	D3D11_BUFFER_DESC d;
	ZeroMemory(&d, sizeof(D3D11_BUFFER_DESC));

	d.Usage = D3D11_USAGE_DYNAMIC;
	d.ByteWidth = size;
	d.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	D3D11_SUBRESOURCE_DATA init;
	ZeroMemory(&init, sizeof(D3D11_SUBRESOURCE_DATA));
	init.pSysMem = data;

	DX::device->CreateBuffer(&d, &init, &buffer);
}

IndexBuffer::~IndexBuffer() {
	buffer->Release();
}

void IndexBuffer::bind() {
	unsigned int offset = 0;
	DX::cxt->IASetIndexBuffer(buffer, format ,0);
}