#pragma once
#include "ResourceManager.h"
#include "Mesh.h"

class MeshManager: public ResourceManager
{
public:
	MeshManager();
	~MeshManager();
	MeshPtr createMeshFromFile(const wchar_t* file_path);
	MeshPtr createMesh(
		VertexMesh* vertex_list_data, UINT vertex_list_size,
		UINT* index_list_data, UINT index_list_size,
		MaterialSlot* material_slot_list, UINT material_slot_list_size
	);
protected:
	// Унаследовано через ResourceManager
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path) override;

};

