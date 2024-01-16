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

	case ImageFormat::RGB32:
		return DXGI_FORMAT_R8G8B8A8_UNORM;

	case ImageFormat::RGBA32:
		return DXGI_FORMAT_R8G8B8A8_UNORM;

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

	D3D11_TEXTURE2D_DESC d3d11TextureDesc;
	memset(&d3d11TextureDesc, 0, sizeof(d3d11TextureDesc));
	d3d11TextureDesc.Width = textureDesc.m_width;
	d3d11TextureDesc.Height = textureDesc.m_height;
	d3d11TextureDesc.MipLevels = (textureDesc.m_mipmapLevel < 1 ? 1 : textureDesc.m_mipmapLevel);
	d3d11TextureDesc.ArraySize = 1;
	d3d11TextureDesc.Format = getDxgiFormat(textureDesc.m_format);
	d3d11TextureDesc.SampleDesc.Count = 1;
	d3d11TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	
	// #TODO: HACK
	if (textureDesc.m_format == ImageFormat::DEPTH32F ||
		textureDesc.m_format == ImageFormat::DEPTH32 ||
		textureDesc.m_format == ImageFormat::DEPTH24)
	{
		d3d11TextureDesc.BindFlags = 0;
	}
	else
	{
		d3d11TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}

	d3d11TextureDesc.CPUAccessFlags = 0;
	d3d11TextureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subresourceData;
	memset(&subresourceData, 0, sizeof(subresourceData));
	subresourceData.pSysMem = subresourceDesc.m_memory;
	subresourceData.SysMemPitch = subresourceDesc.m_memoryPitch;
	subresourceData.SysMemSlicePitch = subresourceDesc.m_memorySlicePitch;

	D3D11_CHECK(device->getD3DDevice()->CreateTexture2D(&d3d11TextureDesc, subresourceDesc.m_memory ? &subresourceData : NULL, &m_texture));

	if (d3d11TextureDesc.BindFlags == D3D11_BIND_SHADER_RESOURCE)
	{
		D3D11_CHECK(device->getD3DDevice()->CreateShaderResourceView(m_texture, NULL, &m_textureSRV));
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