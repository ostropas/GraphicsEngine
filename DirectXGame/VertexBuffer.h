#pragma once
#include <d3d11.h>
#include "Prerequisistes.h"

class VertexBuffer
{
public:
	VertexBuffer(RenderSystem* system);
	bool load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader);
	UINT getSizeVertexList();
	bool release();
	~VertexBuffer();
private:
	UINT m_size_vertex;
	UINT m_size_list;
private:
	ID3D11Buffer* m_buffer;
	ID3D11InputLayout* m_input_layout;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};

