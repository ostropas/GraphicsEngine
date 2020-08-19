#pragma once
#include <d3d11.h>
#include "Prerequisistes.h"

class SwapChain
{
public:
	SwapChain(RenderSystem* system);
	bool init(HWND hwnd, UINT width, UINT height);
	bool present(bool vsync);
	bool release();
	~SwapChain();
private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};

