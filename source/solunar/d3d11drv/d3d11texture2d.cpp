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
	//	return DXGI_FORMAT_D24_UNORM_S8_UINT;
		return DXGI_FORMAT_R24G8_TYPELESS;

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


DXGI_FORMAT ConvertToDepthStencilFormat(DXGI_FORMAT nFormat)
{
	switch (nFormat)
	{
	case DXGI_FORMAT_R24G8_TYPELESS:
		return DXGI_FORMAT_D24_UNORM_S8_UINT;
	case DXGI_FORMAT_R32G8X24_TYPELESS:
		return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		//	case DXGI_FORMAT_R32G32_TYPELESS:   
		//		return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	case DXGI_FORMAT_R32_TYPELESS:
		return DXGI_FORMAT_D32_FLOAT;
	case DXGI_FORMAT_R16_TYPELESS:
		return DXGI_FORMAT_D16_UNORM;
	case DXGI_FORMAT_D32_FLOAT:
		return DXGI_FORMAT_D32_FLOAT; // bypass
	default:
		//ASSERT(false);
		return nFormat;
		break;
	}
}

DXGI_FORMAT ConvertToShaderResourceFmt(DXGI_FORMAT nFormat)
{
	//handle special cases
	switch (nFormat)
	{
	case DXGI_FORMAT_R24G8_TYPELESS:
		return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	case DXGI_FORMAT_R32G8X24_TYPELESS:
		return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		//	case DXGI_FORMAT_R32G32_TYPELESS:   
		//		return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	case DXGI_FORMAT_R32_TYPELESS:
		return DXGI_FORMAT_R32_FLOAT;
	case DXGI_FORMAT_R16_TYPELESS:
		return DXGI_FORMAT_R16_UNORM;
	default:
		return nFormat;
		//pass through		
	}
}

DXGI_FORMAT ConvertToTypelessFmt(DXGI_FORMAT fmt)
{
	switch (fmt)
	{
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		return DXGI_FORMAT_R32G32B32A32_TYPELESS;
		break;

	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		return DXGI_FORMAT_R32G32B32_TYPELESS;
		break;

	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
		return DXGI_FORMAT_R16G16B16A16_TYPELESS;
		break;

	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
		return DXGI_FORMAT_R32G32_TYPELESS;
		break;

	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
		return DXGI_FORMAT_R10G10B10A2_TYPELESS;
		break;

	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
		return DXGI_FORMAT_R8G8B8A8_TYPELESS;
		break;

	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
		return DXGI_FORMAT_BC1_TYPELESS;
		break;

	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
		return DXGI_FORMAT_BC2_TYPELESS;
		break;

	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
		return DXGI_FORMAT_BC3_TYPELESS;
		break;

		// todo: add missing formats if they found required

	default:
		//ASSERT(false);
		return fmt;
	}
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
	d3dTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	bool isDepth =	textureDesc.m_format == ImageFormat::DEPTH32F ||
					textureDesc.m_format == ImageFormat::DEPTH32 ||
					textureDesc.m_format == ImageFormat::DEPTH24;

	if (isDepth)
		d3dTextureDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
	else if (textureDesc.m_renderTargetUsage)
		d3dTextureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;



#if 0
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
#endif


	d3dTextureDesc.CPUAccessFlags = 0;
	d3dTextureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subresourceData;
	memset(&subresourceData, 0, sizeof(subresourceData));
	subresourceData.pSysMem = subresourceDesc.m_memory;
	subresourceData.SysMemPitch = subresourceDesc.m_memoryPitch;
	subresourceData.SysMemSlicePitch = subresourceDesc.m_memorySlicePitch;

	D3D11_CHECK(device->getDevice()->CreateTexture2D(&d3dTextureDesc, subresourceDesc.m_memory ? &subresourceData : NULL, &m_texture));

	if ((d3dTextureDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE ) != 0)  
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		memset(&shaderResourceViewDesc, 0, sizeof(shaderResourceViewDesc));
		DXGI_FORMAT typelessFormat = ConvertToTypelessFmt(d3dTextureDesc.Format);
		DXGI_FORMAT srvFormat = ConvertToShaderResourceFmt(typelessFormat);
		shaderResourceViewDesc.Format = typelessFormat == srvFormat ? d3dTextureDesc.Format : srvFormat;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MipLevels = -1;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

		D3D11_CHECK(device->getDevice()->CreateShaderResourceView(m_texture, &shaderResourceViewDesc, &m_textureSRV));
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