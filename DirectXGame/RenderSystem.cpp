#include "RenderSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"

#include <d3dcompiler.h>

RenderSystem::RenderSystem()
{
}

bool RenderSystem::init()
{
	D3D_DRIVER_TYPE driver_types[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	size_t num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] = {
		D3D_FEATURE_LEVEL_11_0
	};
	size_t num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT result = 0;

	for (size_t driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		result = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels,
			num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);

		if (SUCCEEDED(result))
			break;
		++driver_type_index;
	}

	if (FAILED(result))
		return false;

	m_imm_device_context = new DeviceContext(m_imm_context, this);

	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	return true;
}

bool RenderSystem::release()
{
	if (m_ps)m_ps->Release();

	if (m_psblob)m_psblob->Release();

	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();


	m_imm_device_context->release();
	m_d3d_device->Release();
	return true;
}

RenderSystem::~RenderSystem()
{
}

SwapChain* RenderSystem::createSwapChain()
{
	return new SwapChain(this);
}

DeviceContext* RenderSystem::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBuffer* RenderSystem::createVertexBuffer()
{
	return new VertexBuffer(this);
}

IndexBuffer* RenderSystem::createIndexBuffer()
{
	return new IndexBuffer(this);
}

ConstantBuffer* RenderSystem::createConstantBuffer()
{
	return new ConstantBuffer(this);
}

VertexShader* RenderSystem::createVertexShader(void* shader_byte_code, size_t byte_code_size)
{
	VertexShader* vs = new VertexShader(this);
	if (!vs->init(shader_byte_code, byte_code_size)) {
		vs->release();
		return nullptr;
	}

	return vs;
}

PixelShader* RenderSystem::createPixelShader(void* shader_byte_code, size_t byte_code_size)
{
	PixelShader* ps = new PixelShader(this);
	if (!ps->init(shader_byte_code, byte_code_size)) {
		ps->release();
		return nullptr;
	}

	return ps;
}

bool RenderSystem::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob))) {
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;
}

bool RenderSystem::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob))) {
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;

}

void RenderSystem::releaseCompiledShader()
{
	if (m_blob)m_blob->Release();
}