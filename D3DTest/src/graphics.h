#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3d10.h>
#include <dxgi.h>
#include "input.h"


class DX {
public:
	 
	static IDXGISwapChain* swapchain;
	static ID3D11Device* device;
	static ID3D11DeviceContext* cxt;
	static ID3D11RenderTargetView* backbuffer;
	static ID3D11Texture2D* depthStencil;
	static ID3D11DepthStencilState* depthState;
	static ID3D11DepthStencilView* depthView;
	static HANDLE threadId;

	static HWND hwnd;
	static HINSTANCE instance;
	static int width, height;
	static bool open;
	static bool mouseCaptured;

	static void Init(int w, int h, const char* title, unsigned int ms = 1);
	static void Quit();

	static void SetMouseState(bool capture);
	
	static void Update();
	static void Clear(float r, float g, float b);

	static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
};

unsigned int MouseUpdate(void* p);