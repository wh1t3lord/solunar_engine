#ifndef GRAPHICS_CORE_DEVICE_H
#define GRAPHICS_CORE_DEVICE_H

// need for RenderTargetCreationDesc
#include "graphics/core/rendertarget.h"

namespace solunar {

	class IRenderTarget;
	class ITexture2D;
	class VertexShader;
	class PixelShader;
	class VertexFormat;
	class ISamplerState;

	//! GAPI device interface, close to the DirectX 10 device.
	class IRenderDevice
	{
	public:
		virtual ~IRenderDevice() {}

		//! Create render target.
		virtual IRenderTarget* CreateRenderTarget(const RenderTargetCreationDesc& renderTargetDesc) = 0;

		//! Create buffer (vertex, index, constant and e.x.)
		virtual IBufferBase* CreateBuffer(const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc) = 0;

		//! Create texture 2d.
		virtual ITexture2D* CreateTexture2D(const TextureDesc& textureDesc, const SubresourceDesc& subresourceDesc) = 0;

		//! Create texture sampler state.
		virtual ISamplerState* CreateSamplerState(const SamplerDesc& samplerDesc) = 0;

		//! Set render target.
		virtual void SetRenderTarget(IRenderTarget* rt) = 0;

		//! Set constant buffer.
		virtual void SetConstantBuffer(IBufferBase* cb) = 0;
		virtual void SetConstantBufferIndex(int slot, IBufferBase* cb) = 0;

		//! Set vertex buffer.
		virtual void SetVertexBuffer(IBufferBase* buffer, uint32_t stride, uint32_t offset) = 0;

		//! Set Index buffer.
		virtual void SetIndexBuffer(IBufferBase* buffer, bool use16bitsIndices) = 0;

		//! Set texture 2d.
		virtual void SetTexture2D(int slot, ITexture2D* texture) = 0;

		//! set sampler desc.
		virtual void SetSamplerState(int slot, ISamplerState* sampler) = 0;

		//! Set viewport
		virtual void SetViewport(Viewport* viewport) = 0;

		//! Get current viewport
		virtual Viewport GetViewport() = 0;

		//! Set scissors
		virtual void SetScissors(float x, float y, float w, float h) = 0;

		//! Draw non indexed primitives
		virtual void Draw(PrimitiveMode primitiveMode, size_t verticesStart, size_t verticesCount) = 0;

		//! Draw indexed primitives
		virtual void DrawIndexed(PrimitiveMode primitiveMode, size_t indexStart, size_t indexCount, int baseVertexLocation) = 0;

	};

	extern IRenderDevice* g_renderDevice;
}

#endif // !GRAPHICS_CORE_DEVICE_H
