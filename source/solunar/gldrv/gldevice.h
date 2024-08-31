#ifndef GRAPHICS_GL_GLDEVICE_H
#define GRAPHICS_GL_GLDEVICE_H

#include "graphics/core/device.h"

namespace solunar {

class GLDevice : public IRenderDevice
{
public:
	GLDevice();
	~GLDevice();

	void Create();
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
	void DrawIndexed(PrimitiveMode primitiveMode, size_t indexStart, size_t indexCount, int baseVertexLocation) override;

private:
	Viewport m_viewport;

};

}

#endif // !GRAPHICS_GL_GLDEVICE_H
