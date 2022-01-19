#include "SpaceShooterGame.h"
#include <Windows.h>
#include <algorithm>
#include <Xinput.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include <iostream>
#include "Mesh.h"
#include <time.h>
#include "MathUtils.h"
#include "MeshRenderer.h"

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


SpaceShooterGame::SpaceShooterGame()
{
}

void SpaceShooterGame::render()
{
	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	// Render spaceship
	drawRenderer(m_spaceship);


	for (const auto& asteroid : m_asteroids)
	{
		drawRenderer(asteroid);
	}

	// Render skybox
	m_list_materials.clear();
	m_list_materials.push_back(m_sky_mat);
	drawMesh(m_sky_mesh, m_list_materials);


	m_swap_chain->present(true);


	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	//m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
	m_delta_time = 1.0f / 60.0f;
	m_time += m_delta_time;
}

void SpaceShooterGame::update()
{
	updateSpaceship();
	updateThirdPersonCamera();
	updateLight();
	updateSkyBox();
}

void SpaceShooterGame::updateCamera()
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

void SpaceShooterGame::updateThirdPersonCamera()
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
			if (m_forward > 0.0f) m_cam_distance = 25.0f;
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

	m_cam_pos = m_spaceship->GetTransform()->GetPosition();
	
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

void SpaceShooterGame::updateModel(Vector3D position, Vector3D rotation, Vector3D scale, const std::vector<MaterialPtr>& list_materials)
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

void SpaceShooterGame::updateLight()
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

void SpaceShooterGame::updateSpaceship()
{
	Matrix4x4 world_model, temp;
	world_model.setIdentity();

	m_spaceship_rot.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
	m_spaceship_rot.m_y += m_delta_mouse_x * m_delta_time * 0.1f;

	if (m_spaceship_rot.m_x >= 1.57f)
		m_spaceship_rot.m_x = 1.57f;
	else if (m_spaceship_rot.m_x <= -1.57f)
		m_spaceship_rot.m_x = -1.57f;

	Transform* spaceshipTransform = m_spaceship->GetTransform();

	Vector3D currentRot = spaceshipTransform->GetRotation();
		
	currentRot = Vector3D::lerp(currentRot, m_spaceship_rot, 5.0f * m_delta_time);

	spaceshipTransform->SetRotation(currentRot);

	temp.setIdentity();
	temp.setRotationX(currentRot.m_x);
	world_model *= temp;

	temp.setIdentity();
	temp.setRotationY(currentRot.m_y);
	world_model *= temp;

	m_spaceship_speed = 125.0f;

	if (m_turbo_mode)
		m_spaceship_speed = 305.0f;

	m_spaceship_pos = m_spaceship_pos + world_model.getZDirection() * (m_forward) * m_spaceship_speed*m_delta_time;

	Vector3D currentPos = spaceshipTransform->GetPosition();

	currentPos = Vector3D::lerp(currentPos, m_spaceship_pos, 3.0f * m_delta_time);

	spaceshipTransform->SetPosition(currentPos);
}

void SpaceShooterGame::updateSkyBox()
{
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D(4000.0f, 4000.0f, 4000.0f));
	cc.m_world.setTranslation(m_world_cam.getTranslation());
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;

	m_sky_mat->setData(&cc, sizeof(constant));
}

void SpaceShooterGame::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials)
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

void SpaceShooterGame::drawRenderer(const MeshRendererPtr& meshRenderer)
{
	auto transform = meshRenderer->GetTransform();
	updateModel(transform->GetPosition(), transform->GetRotation(), transform->GetScale(), meshRenderer->GetMaterials());
	drawMesh(meshRenderer->GetMesh(), meshRenderer->GetMaterials());
}

void SpaceShooterGame::resetMousePosition()
{
	RECT win_rect = getClientWindowRect();
	
	int width = (win_rect.right - win_rect.left);
	int height = (win_rect.bottom - win_rect.top);

	InputSystem::get()->setCursorPosition(Point(win_rect.left + width / 2, win_rect.top + height / 2));
}


SpaceShooterGame::~SpaceShooterGame()
{
}

void SpaceShooterGame::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListner(this);

	m_play_state = true;
	this->resetMousePosition();
	InputSystem::get()->showCursor(!m_play_state);

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	srand(static_cast<unsigned>(time(nullptr)));
	

	m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stars_map.jpg");
	m_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");

	m_base_mat = GraphicsEngine::get()->createMaterial(L"DirectionalLightVertexShader.hlsl", L"DirectionalLightPixelShader.hlsl");
	m_base_mat->setCullMode(CULL_MODE_BACK);

	m_sky_mat = GraphicsEngine::get()->createMaterial(L"SkyBoxVertexShader.hlsl", L"SkyBoxPixelShader.hlsl");
	m_sky_mat->addTexture(m_sky_tex);
	m_sky_mat->setCullMode(CULL_MODE_FRONT);


	TexturePtr spaceship_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\spaceship.jpg");
	MeshPtr spaceship_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\spaceship.obj");

	MaterialPtr spaceship_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	spaceship_mat->addTexture(spaceship_tex);
	spaceship_mat->setCullMode(CULL_MODE_BACK);

	std::vector<MaterialPtr> spaceship_materials { spaceship_mat };
	m_spaceship = std::make_shared<MeshRenderer>(Transform(), spaceship_mesh, spaceship_materials);

	TexturePtr asteroid_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\asteroid.jpg");
	MeshPtr asteroid_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\asteroid.obj");

	MaterialPtr asteroid_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	asteroid_mat->addTexture(asteroid_tex);
	asteroid_mat->setCullMode(CULL_MODE_BACK);

	std::vector<MaterialPtr> list_materials{ asteroid_mat };


	for (unsigned int i = 0; i < 200; i++)
	{
		Transform transform = Transform();

		transform.SetPosition(Vector3D(rand() % 4000 + (-2000), rand() % 4000 + (-2000), rand() % 4000 + (-2000)));
		transform.SetRotation(Vector3D((rand() % 628) / 100.0f, (rand() % 628) / 100.0f, (rand() % 628) / 100.0f));
		const float scale = rand() % 20 + 6;
		transform.SetScale(Vector3D(scale, scale, scale));

		MeshRendererPtr meshRenderer = std::make_shared<MeshRenderer>(transform, asteroid_mesh, list_materials);

		m_asteroids.push_back(meshRenderer);
	}

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	m_list_materials.reserve(32);
}

void SpaceShooterGame::onUpdate()
{
	Window::onUpdate();
	InputSystem::get()->update();
	update();
	this->render();

	m_delta_mouse_x = 0;
	m_delta_mouse_y = 0;
}

void SpaceShooterGame::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->setFullScreen(false, 1, 1);
}

void SpaceShooterGame::onFocus()
{
	InputSystem::get()->addListner(this);
}

void SpaceShooterGame::onKillFocus()
{
	InputSystem::get()->removeListner(this);
}

void SpaceShooterGame::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swap_chain->resize(rc.right - rc.left, rc.bottom - rc.top);
	this->update();
	this->render();
}

void SpaceShooterGame::onKeyDown(int key)
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

void SpaceShooterGame::onKeyUp(int key)
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

void SpaceShooterGame::onMouseMove(const Point& mouse_pos)
{
	if (!m_play_state) return;

	RECT win_size = getClientWindowRect();

	int width = (win_size.right - win_size.left);	
	int height = (win_size.bottom - win_size.top);

	m_delta_mouse_x = static_cast<int>(mouse_pos.m_x - (win_size.left + (width / 2.0f)));
	m_delta_mouse_y = static_cast<int>(mouse_pos.m_y - (win_size.top + (height / 2.0f)));

	resetMousePosition();
}

void SpaceShooterGame::onLeftMouseDown(const Point& mouse_pos)
{
	if (m_play_state)
		return;
	
	m_play_state = true;
	InputSystem::get()->showCursor(!m_play_state);
	resetMousePosition();
}

void SpaceShooterGame::onLeftMouseUp(const Point& mouse_pos)
{
}

void SpaceShooterGame::onRightMouseDown(const Point& mouse_pos)
{
}

void SpaceShooterGame::onRightMouseUp(const Point& mouse_pos)
{
}

void SpaceShooterGame::onGamepadKeyDown(WORD key)
{
}

void SpaceShooterGame::onLeftStickMove(const Point& stick_pos)
{
}	

void SpaceShooterGame::onRightStickMove(const Point& stick_pos)
{
}
