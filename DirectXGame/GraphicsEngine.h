#pragma once
#include <d3d11.h>
#include "Prerequisistes.h"
#include "RenderSystem.h"

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();
	// Initialize the GraphicsEngine and Directx11 Device
	bool init();
	// Release all resources loaded
	bool release();
	RenderSystem* getRenderSystem();
public:
	static GraphicsEngine* get();
private:
	RenderSystem* m_render_system = nullptr;
};

