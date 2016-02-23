#include "shader.h"
#include <stdio.h>
#include <iostream>

Shader::Shader(const char* vertex, const char* pixel) {
	ID3DBlob* v = nullptr;
	ID3DBlob* p = nullptr;
	ID3DBlob* error = nullptr;

	wchar_t name[256];
	memset(name, 0, sizeof(name));

	swprintf(name, L"%S", vertex);

	bool res = true;

	int result = D3DCompileFromFile(name, 0, 0, "main", "vs_4_0", 0, 0, &v, &error);

	if (FAILED(result)) {
		if (error) {
			printf("%s\n", error->GetBufferPointer());
			MessageBoxA(NULL, (LPCSTR)error->GetBufferPointer(), "Shader Error", 0);
			error->Release();
			res = false;
		}
	}

	memset(name, 0, sizeof(name));
	swprintf(name, L"%S", pixel);


	result = D3DCompileFromFile(name, 0, 0, "main", "ps_4_0", 0, 0, &p, &error);

	if (FAILED(result)) {
		if (error) {
			printf("%s\n", error->GetBufferPointer());
			MessageBoxA(NULL, (LPCSTR)error->GetBufferPointer(), "Shader Error", 0);
			error->Release();
			res = false;
		}
	}

	if (res) Init(v->GetBufferPointer(), v->GetBufferSize(), p->GetBufferPointer(), p->GetBufferSize());
	else printf("\nFailed To Compile Shaders!\n");

	v->Release();
	p->Release();

}

Shader::~Shader() {
	vs->Release();
	ps->Release();

	delete[] vCode.byteCode, pCode.byteCode;
}

Shader::Shader(void* vertex, size_t vsize, void* pixel, size_t psize) {
	Init(vertex, vsize, pixel, psize);
}

void Shader::Init(void* v, size_t vsize, void* p, size_t psize) {
	vCode.size = vsize;
	vCode.byteCode = new unsigned char[vsize];
	memcpy(vCode.byteCode, v, vsize);

	pCode.size = psize;
	pCode.byteCode = new unsigned char[psize];
	memcpy(pCode.byteCode, p, psize);

	DX::device->CreateVertexShader((const void*)v, vsize, 0, &vs);
	DX::device->CreatePixelShader((const void*)p, psize, 0, &ps);

}

void Shader::bind() {
	DX::cxt->VSSetShader(vs, 0, 0);
	DX::cxt->PSSetShader(ps, 0, 0);
}

void Shader::VSPassBuffers(unsigned int buffer_id, size_t size, void* data) {
	
	ID3D11Buffer* buffer;


	D3D11_BUFFER_DESC bsc;
	ZeroMemory(&bsc, sizeof(D3D11_BUFFER_DESC));
	bsc.Usage = D3D11_USAGE_DYNAMIC;
	bsc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bsc.ByteWidth = size;
	bsc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	D3D11_SUBRESOURCE_DATA init;
	ZeroMemory(&init, sizeof(D3D11_SUBRESOURCE_DATA));
	init.pSysMem = data;

	DX::device->CreateBuffer(&bsc, &init, &buffer);

	DX::cxt->VSSetConstantBuffers(buffer_id, 1, &buffer);

	buffer->Release();


}

void Shader::PSPassBuffers(unsigned int buffer_id, size_t size, void* data) {

	ID3D11Buffer* buffer;


	D3D11_BUFFER_DESC bsc;
	ZeroMemory(&bsc, sizeof(D3D11_BUFFER_DESC));
	bsc.Usage = D3D11_USAGE_DYNAMIC;
	bsc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bsc.ByteWidth = size;
	bsc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA init;
	ZeroMemory(&init, sizeof(D3D11_SUBRESOURCE_DATA));
	init.pSysMem = data;

	int hr = DX::device->CreateBuffer(&bsc, &init, &buffer);

	DX::cxt->PSSetConstantBuffers(buffer_id, 1, &buffer);

	buffer->Release();
}