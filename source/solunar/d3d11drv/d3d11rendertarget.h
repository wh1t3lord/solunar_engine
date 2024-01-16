#ifndef GRAPHICS_D3D11_D3D11RENDERTARGET_H
#define GRAPHICS_D3D11_D3D11RENDERTARGET_H

#include "core/utils/fixedarray.h"

#include "graphics/core/rendertarget.h"
#include "d3d11drv/d3d11device.h"

namespace engine
{

class D3D11RenderTarget : public IRenderTarget
{
public:
	D3D11RenderTarget(D3D11Device* device, const RenderTargetCreationDesc& renderTargetDesc);
	~D3D11RenderTarget();

	void create(D3D11Device* device, const RenderTargetCreationDesc& renderTargetDesc);
	void release() override;

	void bind(D3D11Device* device);

private:
	std::array<ID3D11RenderTargetView*, kMaxRenderTargetTextures> m_renderTargetViews;
	std::array<ID3D11ShaderResourceView*, kMaxRenderTargetTextures> m_shaderResourceViews;

	RenderTargetCreationDesc m_renderTargetDesc;

	ID3D11DepthStencilView* m_depthStencilView;

	uint32_t getHandle() override { return 0; }
};

}

#endif