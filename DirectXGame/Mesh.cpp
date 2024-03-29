#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <Windows.h>
#include <locale>
#include <codecvt>

#include "GraphicsEngine.h"
#include "VertexMesh.h"

Mesh::Mesh(const wchar_t* full_path) : Resource(full_path)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	std::wstring wstr = full_path;

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string inputFile(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &inputFile[0], size_needed, NULL, NULL);

	std::string mtldir = inputFile.substr(0, inputFile.find_last_of("\\/"));

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputFile.c_str(), mtldir.c_str());

	if (!err.empty() || !res) {
		throw std::exception("Mesh not created successfuly");
	}

	std::vector<VertexMesh> list_vertices;
	std::vector<unsigned int> list_indices;

	size_t size_vertex_index_list = 0;

	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_vertex_index_list += shapes[s].mesh.indices.size();
	}

	list_vertices.reserve(size_vertex_index_list);
	list_indices.reserve(size_vertex_index_list);

	m_material_slots.resize(materials.size());

	size_t index_global_offset = 0;

	for (size_t m = 0; m < materials.size(); m++)
	{
		m_material_slots[m].start_index = index_global_offset;
		m_material_slots[m].material_id = m;

		for (size_t s = 0; s < shapes.size(); s++)
		{
			size_t index_offset = 0;

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

				if (shapes[s].mesh.material_ids[f] != m) {
					index_offset += num_face_verts;
					continue;
				}

				Vector3D vertices_face[3];
				Vector2D textcoords_face[3];

				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];
					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;
					if (attribs.texcoords.size())
					{
						tx = attribs.texcoords[index.texcoord_index * 2 + 0];
						ty = attribs.texcoords[index.texcoord_index * 2 + 1];
					}
					vertices_face[v] = Vector3D(vx, vy, vz);
					textcoords_face[v] = Vector2D(tx, ty);
				}

				Vector3D tangent, binormal;

				computeTangents(
					vertices_face[0], vertices_face[1], vertices_face[2],
					textcoords_face[0], textcoords_face[1], textcoords_face[2],
					tangent,binormal
				);

				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];
					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;
					if (attribs.texcoords.size())
					{
						tx = attribs.texcoords[index.texcoord_index * 2 + 0];
						ty = attribs.texcoords[index.texcoord_index * 2 + 1];
					}

					tinyobj::real_t nx = attribs.normals[index.normal_index * 3 + 0];
					tinyobj::real_t ny = attribs.normals[index.normal_index * 3 + 1];
					tinyobj::real_t nz = attribs.normals[index.normal_index * 3 + 2];


					Vector3D v_tangent, v_binormal;
					v_binormal = Vector3D::cross(Vector3D(nx, ny, nz), tangent);
					v_tangent = Vector3D::cross(v_binormal, Vector3D(nx, ny, nz));

					VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz), v_tangent, v_binormal);
					list_vertices.push_back(vertex);

					list_indices.push_back(index_global_offset + v);
				}

				index_offset += num_face_verts;
				index_global_offset += num_face_verts;
			}
		}

		m_material_slots[m].num_indices = index_global_offset - m_material_slots[m].start_index;
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);

	m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&list_vertices[0], sizeof(VertexMesh),
		(UINT)list_vertices.size(), shader_byte_code, (UINT)size_shader);

	m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());
}

Mesh::Mesh(VertexMesh* vertex_list_data, UINT vertex_list_size, UINT* index_list_data, UINT index_list_size,
	MaterialSlot* material_slot_list, UINT material_slot_list_size) : Resource(L"")
{
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);

	m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(
		vertex_list_data, sizeof(VertexMesh),
		vertex_list_size, shader_byte_code, (UINT)size_shader);

	m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list_data, index_list_size);

	m_material_slots.resize(material_slot_list_size);

	for (UINT i = 0; i < material_slot_list_size; i++)
	{
		m_material_slots[i] = material_slot_list[i];
	}
}

Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return m_index_buffer;
}

const MaterialSlot& Mesh::GetMaterialSlot(unsigned int slot)
{
	if (slot >= m_material_slots.size()) return MaterialSlot();
	return m_material_slots[slot];
}

const size_t Mesh::GetMaterialSlots()
{
	return m_material_slots.size();
}

void Mesh::computeTangents(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2,
						   const Vector2D& t0, const Vector2D& t1, const Vector2D& t2,
						   Vector3D& tangent, Vector3D& binormal)

{
	Vector3D deltaPos1 = v1 - v0;
	Vector3D deltaPos2 = v2 - v0;

	Vector2D deltaPosUV1 = t1 - t0;
	Vector2D deltaPosUV2 = t2 - t0;

	float r = 1.0f / (deltaPosUV1.m_x * deltaPosUV2.m_y - deltaPosUV1.m_y * deltaPosUV2.m_x);
	tangent = (deltaPos1 * deltaPosUV2.m_y - deltaPos2 * deltaPosUV1.m_y);
	tangent = Vector3D::normalize(tangent);
	binormal = (deltaPos2 * deltaPosUV1.m_x - deltaPos1 * deltaPosUV1.m_x);
	binormal = Vector3D::normalize(binormal);

}
