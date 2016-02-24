#include "graphics.h"
#include <stdio.h>


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d10.lib")

IDXGISwapChain* DX::swapchain;
ID3D11Device* DX::device;
ID3D11DeviceContext* DX::cxt;
ID3D11RenderTargetView* DX::backbuffer;
ID3D11Texture2D* DX::depthStencil;
ID3D11DepthStencilState* DX::depthState;
ID3D11DepthStencilView* DX::depthView;

HWND DX::hwnd;
HINSTANCE DX::instance;
int DX::width, DX::height;
bool DX::open = false;
bool DX::mouseCaptured = false;
HANDLE DX::threadId = 0;

void DX::Init(int w, int h, const char* title, unsigned int ms) {
	DX::width = w;
	DX::height = h;
	instance = NULL;
	
	WNDCLASSEX ws;
	memset(&ws, 0, sizeof(WNDCLASSEX));

	ws.cbSize = sizeof(WNDCLASSEX);
	ws.hCursor = LoadCursor(NULL, IDC_ARROW);
	ws.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	ws.lpfnWndProc = (WNDPROC)WndProc;
	ws.lpszClassName = "WindowClass";
	ws.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassEx(&ws);

	RECT r = {0, 0, w, h};
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

	hwnd = CreateWindowEx(0, "WindowClass", title, WS_OVERLAPPEDWINDOW, 100, 100, r.right - r.left, r.bottom - r.top, 0, 0, 0, 0);

	ShowWindow(hwnd, SW_SHOW);

	open = true;

	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = w;
	scd.BufferDesc.Height = h;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = ms;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = true;

	#ifdef _DEBUG
	D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG, 0, 0, D3D11_SDK_VERSION, &scd, &swapchain, &device, 0, &cxt);
	#else
	D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0, D3D11_SDK_VERSION, &scd, &swapchain, &device, 0, &cxt);
	#endif

	ID3D11Texture2D* tmp;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&tmp);

	device->CreateRenderTargetView(tmp, 0, &backbuffer);

	tmp->Release();

	//cxt->OMSetRenderTargets(1, &backbuffer, 0);


	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));

	td.ArraySize = 1;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.Format = DXGI_FORMAT_D32_FLOAT;
	td.Height = h;
	td.Width = w;
	td.MipLevels = 1;
	td.SampleDesc.Count = ms;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;

	HRESULT hr = DX::device->CreateTexture2D(&td, 0, &depthStencil);

	if (FAILED(hr))
		printf("CreateTexture2D Failed: 0x%08x\n", hr);

	/*D3D11_DEPTH_STENCIL_DESC ddc;
	ZeroMemory(&ddc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	ddc.DepthEnable = TRUE;
	ddc.DepthFunc = D3D11_COMPARISON_LESS;
	ddc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	ddc.StencilEnable = TRUE;
	ddc.StencilReadMask = 0xFF;
	ddc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	ddc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	ddc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	ddc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	ddc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	ddc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	ddc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	ddc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	ddc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = DX::device->CreateDepthStencilState(&ddc, &depthState);

	if (FAILED(hr))
		printf("CreateDepthStencilState Failed: 0x%08x\n", hr);

	//DX::cxt->OMSetDepthStencilState(depthState,	1);*/

	D3D11_DEPTH_STENCIL_VIEW_DESC vdc;
	ZeroMemory(&vdc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	vdc.Format = DXGI_FORMAT_D32_FLOAT;
	vdc.ViewDimension = ms > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
	vdc.Texture2D.MipSlice = 0;
	
	
	
	hr = DX::device->CreateDepthStencilView(depthStencil, &vdc, &depthView);

	if (FAILED(hr))
		printf("CreateDepthStencilView Failed: 0x%08x\n", hr);


	DX::cxt->OMSetRenderTargets(1, &backbuffer, depthView);

	D3D11_VIEWPORT viewport;

	memset(&viewport, 0, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = w;
	viewport.Height = h;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;

	DX::cxt->RSSetViewports(1, &viewport);

	threadId = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MouseUpdate, 0, 0, 0);

}

void DX::Quit() {
	swapchain->SetFullscreenState(FALSE, 0);
	swapchain->Release();
	backbuffer->Release();
	depthStencil->Release();
//	depthState->Release();
	depthView->Release();
	cxt->Release();
	device->Release();
	CloseWindow(hwnd);
	SuspendThread(threadId);
}

void DX::SetMouseState(bool capture) {
	if (capture && !mouseCaptured) {
		mouseCaptured = true;

		ShowCursor(FALSE);

	} else if (!capture && mouseCaptured) {
		mouseCaptured = false;

		ShowCursor(TRUE);
	}
}

void DX::Update() {
	MSG msg;
	if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	swapchain->Present(0, 0);
}

void DX::Clear(float r, float g, float b) {
	float col[4]{r, g, b, 1.0f};
	cxt->ClearRenderTargetView(backbuffer, col);
	cxt->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
}

LRESULT DX::WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {

	switch (msg) {
		case WM_CLOSE:
			DX::open = false;
		case WM_KEYDOWN:
			Input::keys[w] = true;
			break;
		case WM_KEYUP:
			Input::keys[w] = false;
			Input::prevKeys[w] = false;
			break;
		case WM_MOUSEMOVE:
			Input::mouse_x = LOWORD(l);
			Input::mouse_y = HIWORD(l);
			break;
	}

	return DefWindowProc(hwnd, msg, w, l);
}

unsigned int MouseUpdate(void* p) {
	while (true) {
		if (DX::mouseCaptured) {


			POINT p;
			p.x = DX::width >> 1;
			p.y = DX::height >> 1;

			ClientToScreen(DX::hwnd, &p);

			SetCursorPos(p.x, p.y);
		}

		Sleep(16);
	}
	return 0;
}
