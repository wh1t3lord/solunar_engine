#include "d3d11drv_pch.h"
#include "d3d11drv/d3d11texture2d.h"

namespace engine
{

DXGI_FORMAT getDxgiFormat(ImageFormat format)
{
	switch (format)
	{
	case ImageFormat::RGBA16F:
		return DXGI_FORMAT_R16G16B16A16_FLOAT;

	case ImageFormat::DEPTH24:
		return DXGI_FORMAT_D24_UNORM_S8_UINT;

	case ImageFormat::DEPTH32:
		return DXGI_FORMAT_D32_FLOAT;

	case ImageFormat::R32:
		return DXGI_FORMAT_R8_UNORM;

	case ImageFormat::RGB32:
		return DXGI_FORMAT_R8G8B8A8_UNORM;

	case ImageFormat::RGBA32:
		return DXGI_FORMAT_R8G8B8A8_UNORM;

	case ImageFormat::RGBA32F:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;

	case ImageFormat::RGBA32_UNORM:
		return DXGI_FORMAT_R8G8B8A8_UNORM;

	case ImageFormat::RG32F:
		return DXGI_FORMAT_R32G32_FLOAT;

	default:
		break;
	}

	Assert2(0, "Unknowed image format passed");

	return DXGI_FORMAT_UNKNOWN;
}


D3D11Texture2D::D3D11Texture2D(D3D11Device* device, const TextureDesc& textureDesc, const SubresourceDesc& subresourceDesc) :
	m_textureDesc(textureDesc),
	m_texture(nullptr),
	m_textureSRV(nullptr)
{
	create(device, textureDesc, subresourceDesc);
}

D3D11Texture2D::~D3D11Texture2D()
{
	destroy();
}

void D3D11Texture2D::create(D3D11Device* device, const TextureDesc& textureDesc, const SubresourceDesc& subresourceDesc)
{
	Assert2(device, "Failed to create texture without initialized device.");

	D3D11_TEXTURE2D_DESC d3dTextureDesc;
	memset(&d3dTextureDesc, 0, sizeof(d3dTextureDesc));
	d3dTextureDesc.Width = textureDesc.m_width;
	d3dTextureDesc.Height = textureDesc.m_height;
	d3dTextureDesc.MipLevels = (textureDesc.m_mipmapLevel < 1 ? 1 : textureDesc.m_mipmapLevel);
	d3dTextureDesc.ArraySize = 1;
	d3dTextureDesc.Format = getDxgiFormat(textureDesc.m_format);
	d3dTextureDesc.SampleDesc.Count = 1;
	d3dTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	
	// #TODO: HACK
	if (textureDesc.m_format == ImageFormat::DEPTH32F ||
		textureDesc.m_format == ImageFormat::DEPTH32 ||
		textureDesc.m_format == ImageFormat::DEPTH24)
	{
		d3dTextureDesc.BindFlags = 0;
	}
	else
	{
		d3dTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}

	if (textureDesc.m_renderTargetUsage)
		d3dTextureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;

	d3dTextureDesc.CPUAccessFlags = 0;
	d3dTextureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subresourceData;
	memset(&subresourceData, 0, sizeof(subresourceData));
	subresourceData.pSysMem = subresourceDesc.m_memory;
	subresourceData.SysMemPitch = subresourceDesc.m_memoryPitch;
	subresourceData.SysMemSlicePitch = subresourceDesc.m_memorySlicePitch;

	D3D11_CHECK(device->getDevice()->CreateTexture2D(&d3dTextureDesc, subresourceDesc.m_memory ? &subresourceData : NULL, &m_texture));

	if (d3dTextureDesc.BindFlags == D3D11_BIND_SHADER_RESOURCE)
	{
		D3D11_CHECK(device->getDevice()->CreateShaderResourceView(m_texture, NULL, &m_textureSRV));
	}
}

void D3D11Texture2D::destroy()
{
	if (m_textureSRV)
	{
		m_textureSRV->Release();
		m_textureSRV = nullptr;
	}

	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
}

}