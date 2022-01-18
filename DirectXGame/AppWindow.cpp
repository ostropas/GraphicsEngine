#include "AppWindow.h"
#include <Windows.h>
#include <algorithm>
#include <Xinput.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include <iostream>
#include "Mesh.h"

struct vertex
{
	Vector3D position;
	Vector2D textcoord;
};


__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4D m_light_direction;
	Vector4D m_camera_position;
	Vector4D m_light_position = Vector4D(0,1,0,0);
	float m_light_radius = 4.0f;
	float m_time = 0.0f;
};


AppWindow::AppWindow()
{
}

void AppWindow::render()
{
	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	update();

	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(false);

	TexturePtr list_tex[1];
	list_tex[0] = m_wall_tex;

	drawMesh(m_mesh, m_vs, m_ps, m_cb, list_tex, 1);
	
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(true);

	list_tex[0] = m_sky_tex;
	drawMesh(m_sky_mesh, m_vs, m_sky_ps, m_sky_cb, list_tex, 1);


	m_swap_chain->present(true);


	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
	m_time += m_delta_time;
}

void AppWindow::update()
{
	updateCamera();
	updateModel();
	updateSkyBox();
}

void AppWindow::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;


	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.05f);
	new_pos += world_cam.getXDirection() * (m_rightward * 0.05f);


	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;

	world_cam.inverse();

	m_view_cam = world_cam;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_proj_cam.setPerspectiveFovLH
	(
		1.57f,
		((float)width/(float)height),
		0.1f,
		100.0f
	);

}

void AppWindow::updateModel()
{
	constant cc;

	Matrix4x4 m_light_rot_matrix;
	m_light_rot_matrix.setIdentity();
	m_light_rot_matrix.setRotationY(m_light_rot_y);

	m_light_rot_y += 1.57f * m_delta_time;

	cc.m_world.setIdentity();
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();

	float dist_from_origin = 1.0f;

	cc.m_light_position = Vector4D(cos(m_light_rot_y) * dist_from_origin, 1.0f, sin(m_light_rot_y) * dist_from_origin, 1.0f);

	cc.m_light_radius = m_light_radius;
	cc.m_light_direction = m_light_rot_matrix.getZDirection();
	cc.m_time = m_time;

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::updateSkyBox()
{
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D(100.0f, 100.0f, 100.0f));
	cc.m_world.setTranslation(m_world_cam.getTranslation());
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;

	m_sky_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::drawMesh(const MeshPtr& mesh, const VertexShaderPtr& vs, const PixelShaderPtr& ps, const ConstantBufferPtr& cb, const TexturePtr* list_tex, UINT num_textures)
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(vs, cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(ps, cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(ps);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(ps, list_tex, num_textures);


	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexVuffer());
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());


	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
}

void AppWindow::resetMousePosition()
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	InputSystem::get()->setCursorPosition(Point(width / 2, height / 2));
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListner(this);

	m_play_state = true;
	this->resetMousePosition();
	InputSystem::get()->showCursor(!m_play_state);

	m_wall_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\wall.jpg");

	m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stars_map.jpg");

	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\scene.obj");
	m_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);


	m_world_cam.setTranslation(Vector3D(0, 0, -1.0f));


	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"PointLightVertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PointLightPixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"SkyBoxShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_sky_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant cc;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
	m_sky_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

	m_world_cam.setTranslation(Vector3D(0, 0, -2));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	InputSystem::get()->update();
	this->render();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->setFullScreen(false, 1, 1);
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListner(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListner(this);
}

void AppWindow::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swap_chain->resize(rc.right - rc.left, rc.bottom - rc.top);
	this->render();
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W') {
		m_forward = 1.0f;
	}
	else if (key == 'S') {
		m_forward = -1.0f;
	}
	else if (key == 'A') {
		m_rightward = -1.0f;
	}
	else if (key == 'D') {
		m_rightward = 1.0f;
	}
	else if (key == 'O') {
		m_light_radius -= 1.0f * m_delta_time;
	}
	else if (key == 'P') {
		m_light_radius += 1.0f * m_delta_time;
	}
	//else if (key == 27) { // Esc pressed
	//	::CloseHandle(m_hwnd);
	//}
}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == 'G') {
		m_play_state = !m_play_state;
		InputSystem::get()->showCursor(!m_play_state);
		this->resetMousePosition();
	}
	else if (key == 'F') {
		m_fullscreen_state = !m_fullscreen_state;
		RECT rc = this->getScreenSize();
		m_swap_chain->setFullScreen(m_fullscreen_state, rc.right, rc.bottom);
	}
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	if (!m_play_state) return;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time * 0.1f;

	InputSystem::get()->setCursorPosition(Point(width / 2, height / 2));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_new_scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_new_scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_new_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_new_scale_cube = 1.0f;
}

void AppWindow::onGamepadKeyDown(WORD key)
{
}

void AppWindow::onLeftStickMove(const Point& stick_pos)
{
	m_forward = (float)stick_pos.m_y / 100;
	m_rightward = (float)stick_pos.m_x / 100;

	if (abs(m_forward) <= 0.5f)
		m_forward = 0.0f;

	if (abs(m_rightward) <= 0.5f)
		m_rightward = 0.0f;

	std::cout << m_forward << " " << m_rightward << std::endl;
}	

void AppWindow::onRightStickMove(const Point& stick_pos)
{
}
