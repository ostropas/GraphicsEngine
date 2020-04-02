#include "AppWindow.h"

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right-rc.left, rc.bottom-rc.top);
}

PingPongValue<float> r { 0, 1, 0.0002f, 0 };
PingPongValue<float> g { 0, 1, 0.0001f, 0.5f };
PingPongValue<float> b { 0, 1, 0.0003f, 1 };
void AppWindow::onUpdate()
{
	Window::onUpdate();
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		r.value, g.value, b.value, 1);

	r.step();
	g.step();
	b.step();
	
	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();

	m_swap_chain->release();
	GraphicsEngine::get()->release();
}
