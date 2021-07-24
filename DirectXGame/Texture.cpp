#include "Texture.h"
#include <DirectXTex.h>
#include "GraphicsEngine.h"

Texture::Texture(const wchar_t* full_path): Resource(full_path)
{
	DirectX::ScratchImage image_data;
	HRESULT res = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image_data);

	if (SUCCEEDED(res))
	{
		res = DirectX::CreateTexture(GraphicsEngine::get()->getRenderSystem()->m_d3d_device, image_data.GetImages(),
			image_data.GetImageCount(), image_data.GetMetadata(), &m_texute);

		if (FAILED(res)) throw std::exception("Texture not created successfuly");

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = image_data.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = image_data.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = image_data.GetMetadata().mipLevels;

		res = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateSamplerState(&sampler_desc, &m_sampler_state);
		if (FAILED(res)) throw std::exception("Texture not created successfully");

		res = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateShaderResourceView(m_texute, &desc, &m_shader_res_view);
		if (FAILED(res)) throw std::exception("Texture not created successfully");
	}
	else
	{
		throw std::exception("Texture not created successfully");
	}
}

Texture::Texture(const Rect& size, Texture::Type type) : Resource(L"")
{
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = size.width;
	tex_desc.Height = size.height;

	switch (type)
	{
	case Texture::Normal:
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		break;
	case Texture::RenderTarget:
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		break;
	case Texture::DepthStencil:
		tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		break;
	default:
		break;
	}

	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;

	auto res = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateTexture2D(&tex_desc, nullptr, (ID3D11Texture2D**)&m_texute);

	if (FAILED(res))
		throw std::exception("Texture not created successfully");

	if (type == Texture::Normal || type == Texture::RenderTarget)
	{
		res = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateShaderResourceView(m_texute, NULL, &m_shader_res_view);
		if (FAILED(res))
			throw std::exception("Texture not created successfully");
	}

	if (type == RenderTarget)
	{
		res = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateRenderTargetView(m_texute, NULL, &m_render_target_view);
		if (FAILED(res))
			throw std::exception("Texture not created successfully");
	}

	if (type == DepthStencil)
	{
		res = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateDepthStencilView(m_texute, NULL, &m_depth_stencil_view);
		if (FAILED(res))
			throw std::exception("Texture not created successfully");
	}

	m_size = size;
	m_type = type;
}

Texture::~Texture()
{
	if (m_texute) m_texute->Release();
	if (m_shader_res_view) m_shader_res_view->Release();
	if (m_render_target_view) m_render_target_view->Release();
	if (m_depth_stencil_view) m_depth_stencil_view->Release();
	if (m_sampler_state) m_sampler_state->Release();
}

Rect Texture::getSize() const
{
	return m_size;
}

Texture::Type Texture::getType() const
{
	return m_type;
}
