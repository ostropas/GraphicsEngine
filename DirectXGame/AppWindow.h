#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "PingPongValue.cpp"
#include "VertexBuffer.h"
#include "VertexShader.h"

template class PingPongValue<float>;

class AppWindow : public Window
{
public:
	// Унаследовано через Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
private:
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
	VertexShader* m_vs;
};

