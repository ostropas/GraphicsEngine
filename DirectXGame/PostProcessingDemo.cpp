﻿#include "PostProcessingDemo.h"
#include <Windows.h>
#include <algorithm>
#include <Xinput.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include <iostream>
#include "Mesh.h"
#include <math.h>
#include <time.h>
#include "MathUtils.h"

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


__declspec(align(16))
struct DistortionEffectData
{
	float m_distortion_level = 0.0f;
};


PostProcessingDemo::PostProcessingDemo()
{
}

void PostProcessingDemo::render()
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_render_target,
		0, 0.3f, 0.4f, 1);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearDepthStencil(this->m_depth_stencil);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setRenderTarget(m_render_target, m_depth_stencil);

	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	Rect viewport_size = m_render_target->getSize();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(viewport_size.width, viewport_size.height);


	// Render spaceship
	m_list_materials.clear();
	m_list_materials.push_back(m_spaceship_mat);
	updateModel(m_current_spaceship_pos,m_current_spaceship_rot,Vector3D(1,1,1), m_list_materials);
	drawMesh(m_spaceship_mesh, m_list_materials);

	// Render asteroids
	m_list_materials.clear();
	m_list_materials.push_back(m_asteroid_mat);
	for (unsigned int i = 0; i < 200; i++)
	{
		updateModel(m_asteroids_pos[i],m_asteroids_rot[i], m_asteroids_scale[i], m_list_materials);
		drawMesh(m_asteroid_mesh, m_list_materials);
	}

	// Render skybox
	m_list_materials.clear();
	m_list_materials.push_back(m_sky_mat);
	drawMesh(m_sky_mesh, m_list_materials);

	
	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	DistortionEffectData effect_data = { m_distortion_level };
	
	m_list_materials.clear();
	m_list_materials.push_back(m_post_process_mat);
	m_post_process_mat->setData(&effect_data, sizeof(DistortionEffectData));
	drawMesh(m_quad_mesh, m_list_materials);
	

	m_swap_chain->present(true);


	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	//m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
	m_delta_time = 1.0f / 60.0f;
	m_time += m_delta_time;
}

void PostProcessingDemo::update()
{
	updateSpaceship();
	updateThirdPersonCamera();
	updateLight();
	updateSkyBox();
}

void PostProcessingDemo::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();


	m_cam_rot.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
	m_cam_rot.m_y += m_delta_mouse_x * m_delta_time * 0.1f;

	temp.setIdentity();
	temp.setRotationX(m_cam_rot.m_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_cam_rot.m_y);
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
		5000.0f
	);

}

void PostProcessingDemo::updateThirdPersonCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	m_cam_rot.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
	m_cam_rot.m_y += m_delta_mouse_x * m_delta_time * 0.1f;

	if (m_cam_rot.m_x >= 1.57f)
		m_cam_rot.m_x = 1.57f;
	else if (m_cam_rot.m_x <= -1.57f)
		m_cam_rot.m_x = -1.57f;
	
	m_current_cam_rot = Vector3D::lerp(m_current_cam_rot, m_cam_rot, 3.0f * m_delta_time);

	temp.setIdentity();
	temp.setRotationX(m_current_cam_rot.m_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_current_cam_rot.m_y);
	world_cam *= temp;

	if (static_cast<bool>(m_forward))
	{
		if (m_turbo_mode)
		{
			if (m_forward > 0.0f) m_cam_distance = 19.0f;
			else m_cam_distance = 5.0f;
		}
		else
		{
			if (m_forward > 0.0f) m_cam_distance = 16.0f;
			else m_cam_distance = 9.0f;
		}
	}
	else
	{
		m_cam_distance = 14.0f;
	}

	m_current_cam_distance = lerp(m_current_cam_distance, m_cam_distance, 2.0f * m_delta_time);

	m_cam_pos = m_current_spaceship_pos;
	
	Vector3D new_pos = m_cam_pos + world_cam.getZDirection() * (-m_current_cam_distance);
	new_pos += world_cam.getYDirection() * 5;


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
		5000.0f
	);
}

void PostProcessingDemo::updateModel(Vector3D position, Vector3D rotation, Vector3D scale, const std::vector<MaterialPtr>& list_materials)
{
	constant cc;

	Matrix4x4 temp;
	cc.m_world.setIdentity();

	temp.setIdentity();
	temp.setScale(scale);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(rotation.m_x);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(rotation.m_y);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationZ(rotation.m_z);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setTranslation(position);
	cc.m_world *= temp;

	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();

	cc.m_light_position = m_light_position;
	cc.m_light_radius = 0.0f;
	cc.m_light_direction = m_light_rot_matrix.getZDirection();
	cc.m_time = m_time;

	for (size_t m = 0; m < list_materials.size(); m++)
	{
		list_materials[m]->setData(&cc, sizeof(constant));
	}
}

void PostProcessingDemo::updateLight()
{
	Matrix4x4 temp;
	m_light_rot_matrix.setIdentity();

	temp.setIdentity();
	temp.setRotationX(-0.707f);
	m_light_rot_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(0.707f);
	m_light_rot_matrix *= temp;
}

void PostProcessingDemo::updateSpaceship()
{
	Matrix4x4 world_model, temp;
	world_model.setIdentity();

	m_spaceship_rot.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
	m_spaceship_rot.m_y += m_delta_mouse_x * m_delta_time * 0.1f;

	if (m_spaceship_rot.m_x >= 1.57f)
		m_spaceship_rot.m_x = 1.57f;
	else if (m_spaceship_rot.m_x <= -1.57f)
		m_spaceship_rot.m_x = -1.57f;
	
	m_current_spaceship_rot = Vector3D::lerp(m_current_spaceship_rot, m_spaceship_rot, 5.0f * m_delta_time);

	temp.setIdentity();
	temp.setRotationX(m_current_spaceship_rot.m_x);
	world_model *= temp;

	temp.setIdentity();
	temp.setRotationY(m_current_spaceship_rot.m_y);
	world_model *= temp;

	m_spaceship_speed = 125.0f;

	if (m_turbo_mode)
	{
		m_spaceship_speed = 305.0f;

		if (m_forward != 0)
		{
			m_distortion_level -= m_delta_time * 0.3f;
		}
	}
	else
	{
		m_distortion_level += m_delta_time * 0.3f;
	}

	m_distortion_level = std::clamp(m_distortion_level, 0.7f, 1.0f);

	m_spaceship_pos = m_spaceship_pos + world_model.getZDirection() * (m_forward) * m_spaceship_speed*m_delta_time;

	m_current_spaceship_pos = Vector3D::lerp(m_current_spaceship_pos, m_spaceship_pos, 3.0f * m_delta_time);
}

void PostProcessingDemo::updateSkyBox()
{
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D(4000.0f, 4000.0f, 4000.0f));
	cc.m_world.setTranslation(m_world_cam.getTranslation());
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;

	m_sky_mat->setData(&cc, sizeof(constant));
}

void PostProcessingDemo::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials)
{
	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	for (size_t m = 0; m < list_materials.size(); m++)
	{
		if (m >= list_materials.size()) break;

		MaterialSlot mat = mesh->GetMaterialSlot(m);

		GraphicsEngine::get()->setMaterial(list_materials[m]);

		// FINALLY DRAW THE TRIANGLE
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mat.num_indices, 0, mat.start_index);
	}
}

void PostProcessingDemo::resetMousePosition()
{
	RECT win_rect = getClientWindowRect();
	
	int width = (win_rect.right - win_rect.left);
	int height = (win_rect.bottom - win_rect.top);

	InputSystem::get()->setCursorPosition(Point(win_rect.left + width / 2, win_rect.top + height / 2));
}


PostProcessingDemo::~PostProcessingDemo()
{
}

void PostProcessingDemo::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListner(this);

	m_play_state = true;
	this->resetMousePosition();
	InputSystem::get()->showCursor(!m_play_state);

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	srand(static_cast<unsigned>(time(nullptr)));
	
	for (unsigned int i = 0; i < 200; i++)
	{
		m_asteroids_pos[i] = Vector3D(rand()%4000 + (-2000), rand() % 4000 + (-2000), rand() % 4000 + (-2000));
		m_asteroids_rot[i] = Vector3D((rand()%628)/100.0f, (rand() % 628) / 100.0f, (rand() % 628) / 100.0f);
		const float scale = rand() % 20 + 6;
		m_asteroids_scale[i] = Vector3D(scale, scale, scale);
	}

	m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stars_map.jpg");
	m_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");

	m_spaceship_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\spaceship.jpg");
	m_spaceship_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\spaceship.obj");

	m_asteroid_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\asteroid.jpg");
	m_asteroid_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\asteroid.obj");

	m_base_mat = GraphicsEngine::get()->createMaterial(L"DirectionalLightVertexShader.hlsl", L"DirectionalLightPixelShader.hlsl");
	m_base_mat->setCullMode(CULL_MODE_BACK);

	m_sky_mat = GraphicsEngine::get()->createMaterial(L"SkyBoxVertexShader.hlsl", L"SkyBoxPixelShader.hlsl");
	m_sky_mat->addTexture(m_sky_tex);
	m_sky_mat->setCullMode(CULL_MODE_FRONT);

	m_spaceship_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	m_spaceship_mat->addTexture(m_spaceship_tex);
	m_spaceship_mat->setCullMode(CULL_MODE_BACK);

	m_asteroid_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	m_asteroid_mat ->addTexture(m_asteroid_tex);
	m_asteroid_mat ->setCullMode(CULL_MODE_BACK);

	m_post_process_mat = GraphicsEngine::get()->createMaterial(L"PostProcessVS.hlsl", L"DistortionEffect.hlsl");
	m_post_process_mat ->setCullMode(CULL_MODE_BACK);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	VertexMesh quad_vertex_list[] = {
		VertexMesh(Vector3D(-1,-1,0), Vector2D(0, 1), Vector3D(), Vector3D(), Vector3D()),
		VertexMesh(Vector3D(-1,1,0), Vector2D(0, 0), Vector3D(), Vector3D(), Vector3D()),
		VertexMesh(Vector3D(1,1,0), Vector2D(1, 0), Vector3D(), Vector3D(), Vector3D()),
		VertexMesh(Vector3D(1,-1,0), Vector2D(1, 1), Vector3D(), Vector3D(), Vector3D())
	};

	UINT quad_index_list[] = {
		0,1,2,
		2,3,0,
		
	};

	MaterialSlot quad_mat_slots[] = {
		{0,6,0}
	};

	m_quad_mesh = GraphicsEngine::get()->getMeshManager()->createMesh(
		quad_vertex_list, 4,
		quad_index_list, 6,
		quad_mat_slots, 1
	);

	m_list_materials.reserve(32);

	m_render_target = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(rc.right - rc.left, rc.bottom - rc.top), Texture::RenderTarget);
	m_depth_stencil = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(rc.right - rc.left, rc.bottom - rc.top), Texture::DepthStencil);

	m_post_process_mat->addTexture(m_render_target);
}

void PostProcessingDemo::onUpdate()
{
	Window::onUpdate();
	InputSystem::get()->update();
	update();
	this->render();

	m_delta_mouse_x = 0;
	m_delta_mouse_y = 0;
}

void PostProcessingDemo::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->setFullScreen(false, 1, 1);
}

void PostProcessingDemo::onFocus()
{
	InputSystem::get()->addListner(this);
}

void PostProcessingDemo::onKillFocus()
{
	InputSystem::get()->removeListner(this);
}

void PostProcessingDemo::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swap_chain->resize(rc.right - rc.left, rc.bottom - rc.top);

	m_render_target = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(rc.right - rc.left, rc.bottom - rc.top), Texture::RenderTarget);
	m_depth_stencil = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(rc.right - rc.left, rc.bottom - rc.top), Texture::DepthStencil);
	m_post_process_mat->removeTexture(0);
	m_post_process_mat->addTexture(m_render_target);

	this->update();
	this->render();
}

void PostProcessingDemo::onKeyDown(int key)
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
	else if (key == VK_SHIFT)
	{
		m_turbo_mode = true;
	}
}

void PostProcessingDemo::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == VK_ESCAPE) {
		if (!m_play_state)
			return;
		
		m_play_state = false;
		InputSystem::get()->showCursor(!m_play_state);
		resetMousePosition();
	}
	else if (key == 'F') {
		m_fullscreen_state = !m_fullscreen_state;
		RECT rc = this->getScreenSize();
		m_swap_chain->setFullScreen(m_fullscreen_state, rc.right, rc.bottom);
	}
	else if (key == VK_SHIFT)
	{
		m_turbo_mode = false;
	}
}

void PostProcessingDemo::onMouseMove(const Point& mouse_pos)
{
	if (!m_play_state) return;

	RECT win_size = getClientWindowRect();

	int width = (win_size.right - win_size.left);	
	int height = (win_size.bottom - win_size.top);

	m_delta_mouse_x = static_cast<int>(mouse_pos.m_x - (win_size.left + (width / 2.0f)));
	m_delta_mouse_y = static_cast<int>(mouse_pos.m_y - (win_size.top + (height / 2.0f)));

	resetMousePosition();
}

void PostProcessingDemo::onLeftMouseDown(const Point& mouse_pos)
{
	if (m_play_state)
		return;
	
	m_play_state = true;
	InputSystem::get()->showCursor(!m_play_state);
	resetMousePosition();
}

void PostProcessingDemo::onLeftMouseUp(const Point& mouse_pos)
{
}

void PostProcessingDemo::onRightMouseDown(const Point& mouse_pos)
{
}

void PostProcessingDemo::onRightMouseUp(const Point& mouse_pos)
{
}

void PostProcessingDemo::onGamepadKeyDown(WORD key)
{
}

void PostProcessingDemo::onLeftStickMove(const Point& stick_pos)
{
	m_forward = (float)stick_pos.m_y / 100;
	m_rightward = (float)stick_pos.m_x / 100;

	if (abs(m_forward) <= 0.5f)
		m_forward = 0.0f;

	if (abs(m_rightward) <= 0.5f)
		m_rightward = 0.0f;

	std::cout << m_forward << " " << m_rightward << std::endl;
}	

void PostProcessingDemo::onRightStickMove(const Point& stick_pos)
{
}
