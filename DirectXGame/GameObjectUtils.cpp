#include "GameObjectUtils.h"
#include "GraphicsEngine.h"

void GameObjectUtils::CreateObjectResources(TexturePtr & texture, MaterialPtr & material, MeshPtr & mesh)
{
	texture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stars_map.jpg");
	mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	material = 
}
