#pragma once
#include <d3d11.h>
#include "Prerequisistes.h"
#include "RenderSystem.h"

class GraphicsEngine
{
private:
	// Initialize the GraphicsEngine and Directx11 Device
	GraphicsEngine();
	// Release all resources loaded
	~GraphicsEngine();
public:
	RenderSystem* getRenderSystem();
public:
	static GraphicsEngine* get();
	static void create();
	static void release();
private:
	RenderSystem* m_render_system = nullptr;
	static GraphicsEngine* m_engine;
};

