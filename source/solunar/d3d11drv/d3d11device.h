#ifndef GRAPHICS_D3D11_D3D11DEVICE_H
#define GRAPHICS_D3D11_D3D11DEVICE_H

#include "graphics/core/device.h"

namespace solunar
{

class D3D11Device : public IRenderDevice
{
public:
	D3D11Device();
	~D3D11Device();

	//! Create d3d11 device
	void Create();

	//! Destroy d3d11 device
	void Destroy();

	//! Create render target.
	IRenderTarget* CreateRenderTarget(const RenderTargetCreationDesc& renderTargetDesc) override;

	//! Create buffer (vertex, index, constant and e.x.)
	IBufferBase* CreateBuffer(const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc) override;

	//! Create texture 2d.
	ITexture2D* CreateTexture2D(const TextureDesc& textureDesc, const SubresourceDesc& subresourceDesc) override;

	//! Create texture sampler state.
	ISamplerState* CreateSamplerState(const SamplerDesc& samplerDesc) override;

	//! Set render target
	void SetRenderTarget(IRenderTarget* rt) override;

	//! Set constant buffer.
	void SetConstantBuffer(IBufferBase* cb) override;
	void SetConstantBufferIndex(int slot, IBufferBase* cb) override;

	//! Set vertex buffer.
	void SetVertexBuffer(IBufferBase* buffer, uint32_t stride, uint32_t offset) override;

	//! Set Index buffer.
	void SetIndexBuffer(IBufferBase* buffer, bool use16bitsIndices) override;

	//! Set texture 2d.
	void SetTexture2D(int slot, ITexture2D* texture) override;

	//! set sampler desc.
	void SetSamplerState(int slot, ISamplerState* sampler) override;

	//! Set viewport
	void SetViewport(Viewport* viewport) override;

	//! Get current viewport
	Viewport GetViewport() override;

	//! Set scissors
	void SetScissors(float x, float y, float w, float h) override;

	//! Draw non indexed primitives
	void Draw(PrimitiveMode primitiveMode, size_t verticesStart, size_t verticesCount) override;

	//! Draw indexed primitives
	//! NOTE: Unsigned byte indices support only!
	void DrawIndexed(PrimitiveMode primitiveMode, size_t indexStart, size_t indexCount, int baseVertexLocation) override;

public:
	ID3D11Device* GetDevice() { return m_device; }
	ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext; }

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	Viewport m_viewport;
};

#define g_d3d11Device ((D3D11Device*)g_renderDevice)

}

#endif