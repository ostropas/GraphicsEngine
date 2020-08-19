#include "PixelShader.h"
#include "RenderSystem.h"

PixelShader::PixelShader(RenderSystem* system) : m_system(system)
{
}

void PixelShader::release()
{
	m_ps->Release();
	delete this;
}

PixelShader::~PixelShader()
{
}

bool PixelShader::init(const void* shader_byte_code, size_t byte_code_size)
{
	if (!SUCCEEDED(m_system->m_d3d_device->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &m_ps)))
		return false;
	return true;
}
