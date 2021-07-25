#include "MeshManager.h"
#include "Mesh.h"

MeshManager::MeshManager():ResourceManager()
{
}

MeshManager::~MeshManager()
{
}

MeshPtr MeshManager::createMeshFromFile(const wchar_t* file_path)
{
    return std::static_pointer_cast<Mesh>(createResourceFromFile(file_path));
}

MeshPtr MeshManager::createMesh(VertexMesh* vertex_list_data, UINT vertex_list_size, UINT* index_list_data,
	UINT index_list_size, MaterialSlot* material_slot_list, UINT material_slot_list_size)
{
	Mesh* mesh = nullptr;
	try
	{
		mesh = new Mesh(
			vertex_list_data, vertex_list_size,
			index_list_data, index_list_size,
			material_slot_list, material_slot_list_size
		);
	}
	catch (...)
	{
		
	}

	MeshPtr mesh_ptr(mesh);
	return mesh_ptr;
}

Resource* MeshManager::createResourceFromFileConcrete(const wchar_t* file_path)
{
	Mesh* mesh = nullptr;
	try
	{
		mesh = new Mesh(file_path);
	}
	catch (...)
	{
		
	}

	return mesh;
}
