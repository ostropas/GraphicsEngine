#pragma once
#include <d3d11.h>
#include "Prerequisistes.h"

class ConstantBuffer
{
public:
	ConstantBuffer(RenderSystem* system);
	bool load(void* buffer, UINT size_buffer);
	void update(DeviceContext* context, void* buffer);
	bool release();
	~ConstantBuffer();
private:
	ID3D11Buffer* m_buffer;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};

