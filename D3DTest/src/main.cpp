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
#include "entity.h"

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

	Model dragonModel, sphereModel, planeModel;

	ID3D11Resource* res;
	ID3D11ShaderResourceView* view;
	DirectX::CreateWICTextureFromFile(DX::device, DX::cxt, L"res/Gunmetal.jpg", &res, &view, 0);



	loadObjModel("res/dragon.obj", &dragonModel);
	loadObjModel("res/sphere.obj", &sphereModel);
	loadObjModel("res/plane.obj", &planeModel);

	Entity dragon(tomato::math::vec3(0, -1, -3.5f), tomato::math::vec3(0, 0, 0), &dragonModel);
	Entity sphere(tomato::math::vec3(0, 0, 3.5), tomato::math::vec3(0, 0, 0), &sphereModel);
	Entity plane(tomato::math::vec3(0, -1, 0), tomato::math::vec3(0, 0, 0), &planeModel);

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

	struct PointLight {
		__declspec(align(16)) 	tomato::math::vec3 position;
		
		__declspec(align(16)) tomato::math::vec3 color;
		float constant;
		float linear;
		float exponent;
	} point;

	point.color = tomato::math::vec3(1, 0, 1);
	point.color = tomato::math::vec3(0.456, 0.76, 1);
	point.position = tomato::math::vec3(0, 2, 2);
	point.constant = 0.01;
	point.linear = 0.25;
	point.exponent = 0.035f;

	shader.PSPassBuffers(2, sizeof(PointLight), &point);

	__declspec(align(16)) struct Light {
		tomato::math::vec3 lightDir;
	} light;

	light.lightDir = tomato::math::vec3(0, -0.75f, -0.25f);

	shader.PSPassBuffers(1, sizeof(Light), &light);

	shader.VSPassBuffers(0, sizeof(tomato::math::mat4), &tomato::math::mat4().perspective(70.0f, 16.0f / 9.0f, 0.001f, 1000.0f));
	shader.VSPassBuffers(3, sizeof(tomato::math::mat4), &tomato::math::mat4(1));


	tomato::math::vec3 pos(0, -1, -3.5f);
	
	unsigned int time = clock();

	const float ms = 1.0f / 1000.0f;

	float delta = 0;

	//DX::SetMouseState(true);

	float a = 0;
	
	while (DX::open) {
		DX::Clear(0, 0, 0);
	
		delta = ((float)clock() - time) * ms;

		time = clock();

		camera->update(delta);
		a += 1 * delta;

		point.position.x = cosf(a) * 10;
		shader.PSPassBuffers(2, sizeof(PointLight), &point);

		dragon.render(&shader);
		plane.render(&shader);
		sphere.render(&shader);

		/*pos.x = cosf(zRot) * (21.0f / 9.0f);
		pos.z = sinf(zRot) -2.5f;*/
		//r.z += 0.24f;
	//	r.y += 75 * delta;

		zRot -= 0.015f;
		
		DX::Update();
		//Sleep(5);
	}
		
	DX::Quit();

	delete camera;
	return 0;
}

int main() {
	return MAIN();
}

int __stdcall WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR str, int cmd) {
	return MAIN();
}
