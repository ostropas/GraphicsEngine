#pragma once
#include <vector>
#include "Prerequisistes.h"
#include "SceneComponent.h"

class MeshRenderer : public SceneComponent
{
public:
	MeshRenderer(Transform transform,
		MeshPtr& mesh,
		std::vector<MaterialPtr>& list_materials) : m_mesh(mesh), m_list_materials(list_materials), SceneComponent(transform) {}

	MeshPtr GetMesh() { return m_mesh; }
	std::vector<MaterialPtr> GetMaterials() { return m_list_materials; }

private:
	MeshPtr m_mesh;
	std::vector<MaterialPtr> m_list_materials;
};

