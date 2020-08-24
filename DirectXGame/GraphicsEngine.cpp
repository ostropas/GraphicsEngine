#include "GraphicsEngine.h"
#include <exception>

GraphicsEngine* GraphicsEngine::m_engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	try {
		m_render_system = new RenderSystem();
	}
	catch (...) { throw std::exception("RenderSystem not created succesfuly"); }

	try {
		m_tex_manager = new TextureManager();
	}
	catch (...) { throw std::exception("TextureManage not created succesfuly"); }
}

GraphicsEngine::~GraphicsEngine()
{
	GraphicsEngine::m_engine = nullptr;
	delete m_tex_manager;
	delete m_render_system;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return m_render_system;
}

TextureManager* GraphicsEngine::getTextureManager()
{
	return m_tex_manager;
}

GraphicsEngine* GraphicsEngine::get()
{
	return m_engine;
}

void GraphicsEngine::create()
{
	if (GraphicsEngine::m_engine) throw std::exception("Graphics Engine already created");
	GraphicsEngine::m_engine = new GraphicsEngine();
}

void GraphicsEngine::release()
{
	if (!GraphicsEngine::m_engine) return;
	delete GraphicsEngine::m_engine;
}
