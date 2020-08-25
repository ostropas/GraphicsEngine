#include "SwapChain.h"
#include "RenderSystem.h"
#include <exception>

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system) : m_system(system)
{
	ID3D11Device* device = m_system->m_d3d_device;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;

	HRESULT res = m_system->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);

	if (FAILED(res))
		throw std::exception("SwapChain not created successfuly");

	ID3D11Texture2D* buffer = nullptr;
	res = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(res))
		throw std::exception("SwapChain not created successfuly");

	res = device->CreateRenderTargetView(buffer, NULL, &m_rtv);
	buffer->Release();

	if (FAILED(res))
		throw std::exception("SwapChain not created successfuly");

	D3D11_TEXTURE2D_DESC tex_depth = {};
	tex_depth.Width = width;
	tex_depth.Height = height;
	tex_depth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex_depth.Usage = D3D11_USAGE_DEFAULT;
	tex_depth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_depth.MipLevels = 1;
	tex_depth.SampleDesc.Count = 1;
	tex_depth.SampleDesc.Quality = 0;
	tex_depth.MiscFlags = 0;
	tex_depth.ArraySize = 1;
	tex_depth.CPUAccessFlags = 0;

	res = device->CreateTexture2D(&tex_depth, nullptr, &buffer);

	if (FAILED(res))
		throw std::exception("SwapChain not created successfuly");

	res = device->CreateDepthStencilView(buffer, NULL, &m_dsv);
	buffer->Release();

	if (FAILED(res))
		throw std::exception("SwapChain not created successfuly");


}

bool SwapChain::present(bool vsync)
{
	m_swap_chain->Present(vsync, NULL);
	return true;
}

SwapChain::~SwapChain()
{
	m_swap_chain->Release();
}
