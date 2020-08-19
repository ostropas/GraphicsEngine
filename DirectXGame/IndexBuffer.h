#pragma once
#include <d3d11.h>
#include "Prerequisistes.h"

class IndexBuffer
{
public:
	IndexBuffer(RenderSystem* system);
	bool load(void* list_indecies, UINT size_list);
	UINT getSizeIndexList();
	bool release();
	~IndexBuffer();
private:
	UINT m_size_list;
private:
	ID3D11Buffer* m_buffer;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};

