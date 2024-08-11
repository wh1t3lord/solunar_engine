#ifndef GRAPHICS_D3D11_D3D11SAMPLERSTATE_H
#define GRAPHICS_D3D11_D3D11SAMPLERSTATE_H

#include "graphics/core/texture.h"
#include "d3d11drv/d3d11device.h"

namespace solunar
{

class D3D11SamplerState : public ISamplerState
{
public:
	D3D11SamplerState(D3D11Device* device, const SamplerDesc& samplerDesc);
	~D3D11SamplerState();

	void Create(D3D11Device* device, const SamplerDesc& samplerDesc);
	void Destroy();

	uint32_t getHandle() { return m_handle; }

	ID3D11SamplerState* getSamplerState() { return m_samplerState; }

private:
	ID3D11SamplerState* m_samplerState;

	uint32_t m_handle;
};


}

#endif // !GRAPHICS_D3D11_D3D11SAMPLERSTATE_H
