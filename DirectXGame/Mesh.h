#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include <vector>
#include "VertexMesh.h"

struct MaterialSlot
{
	size_t start_index = 0;
	size_t num_indices = 0;
	size_t material_id = 0;
};

class Mesh : public Resource
{
public:
	Mesh(const wchar_t* full_path);
	Mesh(
		VertexMesh* vertex_list_data, UINT vertex_list_size,
		UINT* index_list_data, UINT index_list_size,
		MaterialSlot* material_slot_list, UINT material_slot_list_size
	);
	~Mesh();
	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();

	const MaterialSlot& GetMaterialSlot(unsigned int slot);
	const size_t GetMaterialSlots();
private:
	void computeTangents(
		const Vector3D& v0,	const Vector3D& v1,	const Vector3D& v2,
		const Vector2D& t0,	const Vector2D& t1,	const Vector2D& t2,
		Vector3D& tangent, Vector3D& binormal);
private:
	VertexBufferPtr m_vertex_buffer;
	IndexBufferPtr m_index_buffer;
	std::vector<MaterialSlot> m_material_slots;
private:
	friend class DeviceContext;
};

