#include "graphics.h"
#include "shader.h"
#include <stdio.h>
#include <d3dcompiler.h>
#include "../include/WICTextureLoader.h"


#include "../include/math/math.h"
#include "vertexbuffer.h"
#include "IndexBuffer.h"
#include "objmodel.h"
#include "FlyCamera.h"
#include <ctime>
#include "input.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")
struct Vertex {
	tomato::math::vec3 position; //float x, y, z;
	tomato::math::vec4 color;//float r, g, b, a;
	tomato::math::vec2 uv;//float u, v;
	tomato::math::vec3 normal;
};


ID3D11Texture2D* texture;


void loadBinaryFile(const char* filename, char** data, unsigned int& size) {
	FILE* file = fopen(filename, "rb");

	if (file) {
		
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		fseek(file, 0, SEEK_SET);

		*data = new char[size];

		fread(*data, size, 1, file);

		fclose(file);

	} else {
		printf("NOOOOOOOO!\n");
	}


}


#define DRAW_TRIANGLE 0

int MAIN() {

	Camera* camera = new FlyCamera(tomato::math::vec3(), tomato::math::vec3());

	DX::Init(1280, 720, "Title", 4);

	VertexBuffer* vertex = nullptr;
	IndexBuffer* index = nullptr;
	ID3D11Resource* res;
	ID3D11ShaderResourceView* view;
	DirectX::CreateWICTextureFromFile(DX::device, DX::cxt, L"res/tex.png", &res, &view, 0);



	loadObjModel("res/dragon.obj", &vertex, &index);

	DX::cxt->PSSetShaderResources(1, 1, &view);

	#if 0
	char* vs = nullptr;
	unsigned int vsize;
	char* ps = nullptr;
	unsigned int psize;

	loadBinaryFile("res/VertexShader.cso", &vs, vsize);
	loadBinaryFile("res/PixelShader.cso", &ps, psize);
	Shader shader(vs, vsize, ps, psize);

	delete[] vs, ps;
	#else
	Shader shader("res/VertexShader.hlsl", "res/PixelShader.hlsl");
	#endif
	shader.bind();

	/*Vertex vertices[] = {
		{vec3(0, 1, 0), vec4(1, 0, 0, 1), vec2(0.5f, 0), vec3(0, 0, 1)},
		{vec3(1, -1, 0), vec4(0, 1, 0, 1), vec2(1, 1), vec3(0, 0, 1)},
		{vec3(-1, -1, 0), vec4(0, 0, 1, 1), vec2(0, 1), vec3(0, 0, 1)}
	};

	unsigned int indices[3]{0, 1, 2};

	
	VertexBuffer vbuffer(vertices, sizeof(vertices), sizeof(Vertex));
	IndexBuffer ibuffer(indices, sizeof(indices), DXGI_FORMAT_R32_UINT, 3);

	*/

	D3D11_SAMPLER_DESC sdc;
	ZeroMemory(&sdc, sizeof(D3D11_SAMPLER_DESC));

	sdc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sdc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sdc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sdc.MaxAnisotropy = 16;
	sdc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	ID3D11SamplerState* sampler;
	int result = DX::device->CreateSamplerState(&sdc, &sampler);

	DX::cxt->PSSetSamplers(0, 1, &sampler);

	ID3D11RasterizerState* state;
	D3D11_RASTERIZER_DESC dsc;
	ZeroMemory(&dsc, sizeof(D3D11_RASTERIZER_DESC));

	dsc.FillMode = D3D11_FILL_SOLID;
	dsc.CullMode = D3D11_CULL_BACK;
	#if DRAW_TRIANGLE
	dsc.FrontCounterClockwise = false;
	#else
	dsc.FrontCounterClockwise = true;
	#endif
	dsc.DepthClipEnable = true;
	

	DX::device->CreateRasterizerState(&dsc, &state);

	DX::cxt->RSSetState(state);
	
	ID3D11InputLayout* layout;

	D3D11_INPUT_ELEMENT_DESC desc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	DX::device->CreateInputLayout(desc, 4, shader.getVCode(), shader.getVCodeSize(), &layout);


	DX::cxt->IASetInputLayout(layout);

	#if DRAW_TRIANGLE
	vbuffer.bind();
	ibuffer.bind();
	#else
	vertex->bind();
	index->bind();
	#endif
	DX::cxt->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	float zRot = 0;

	
	
	tomato::math::vec3 r(0, 0, 0);
	
	struct out_data {
		tomato::math::mat4 projection;
		tomato::math::mat4 view;
		tomato::math::mat4 model;
	} out;

	out.projection = tomato::math::mat4().perspective(70.0f, 16.0f / 9.0f, 0.001f, 1000.0f);

	out.model = tomato::math::mat4(1);

	__declspec(align(16)) struct PointLight {
		tomato::math::vec3 position;
		tomato::math::vec3 color;
		float constant;
		float linear;
		float exponent;
	} point;

	point.color = tomato::math::vec3(1, 1, 1);
	point.position = tomato::math::vec3(0, 0, 4);
	point.constant = 1;
	point.linear = 1;
	point.exponent = 1.0f;

	shader.PSPassBuffers(2, sizeof(PointLight), &point);

	__declspec(align(16)) struct Light {
		tomato::math::vec3 lightDir;
	} light;

	light.lightDir = tomato::math::vec3(0, 0, -1);

	shader.PSPassBuffers(1, sizeof(Light), &light);

	tomato::math::vec3 pos(0, -1, -3.5f);
	
	unsigned int time = clock();

	const float ms = 1.0f / 1000.0f;

	float delta = 0;

	//DX::SetMouseState(true);
	
	while (DX::open) {
	
		delta = ((float)clock() - time) * ms;

		time = clock();

		camera->update(delta);

		out.view = camera->getViewMatrix();

		DX::Clear(0, 0, 0);

		shader.VSPassBuffers(0, sizeof(out_data), &out);
		#if DRAW_TRIANGLE
		DX::cxt->DrawIndexed(ibuffer.getCount(), 0, 0);
		#else
		DX::cxt->DrawIndexed(index->getCount(), 0, 0);
		#endif

		DX::Update();

		zRot -= 0.015f;
		
		/*pos.x = cosf(zRot) * (21.0f / 9.0f);
		pos.z = sinf(zRot) -2.5f;*/
		//r.z += 0.24f;
		r.y += 75 * delta;

		out.model = tomato::math::mat4().translate(pos) * tomato::math::mat4().rotate(r);// *mat4().scale(vec3(1.5, 1.5, 1.5));
		
		Sleep(5);
	}
		
	DX::Quit();

	delete vertex;
	delete index;
	delete camera;
	return 0;
}

int main() {
	return MAIN();
}

int __stdcall WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR str, int cmd) {
	return MAIN();
}
