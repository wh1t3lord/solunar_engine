#ifndef GRAPHICS_CORE_DEVICE_H
#define GRAPHICS_CORE_DEVICE_H

// need for RenderTargetCreationDesc
#include "graphics/core/rendertarget.h"

namespace engine {

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
		virtual IRenderTarget* createRenderTarget(const RenderTargetCreationDesc& renderTargetDesc) = 0;

		//! Create buffer (vertex, index, constant and e.x.)
		virtual IBufferBase* createBuffer(const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc) = 0;

		//! Create texture 2d.
		virtual ITexture2D* createTexture2D(const TextureDesc& textureDesc, const SubresourceDesc& subresourceDesc) = 0;

		//! Create texture sampler state.
		virtual ISamplerState* createSamplerState(const SamplerDesc& samplerDesc) = 0;

		//! Set render target.
		virtual void setRenderTarget(IRenderTarget* rt) = 0;

		//! Set constant buffer.
		virtual void setConstantBuffer(IBufferBase* cb) = 0;
		virtual void setConstantBufferIndex(int slot, IBufferBase* cb) = 0;

		//! Set vertex buffer.
		virtual void setVertexBuffer(IBufferBase* buffer, uint32_t stride, uint32_t offset) = 0;

		//! Set Index buffer.
		virtual void setIndexBuffer(IBufferBase* buffer, bool use16bitsIndices) = 0;

		//! Set texture 2d.
		virtual void setTexture2D(int slot, ITexture2D* texture) = 0;

		//! set sampler desc.
		virtual void setSampler(int slot, ISamplerState* sampler) = 0;

		//! Set vertex format.
		virtual void setVertexFormat(VertexFormat* vertex_format) = 0;

		//! Set viewport
		virtual void setViewport(Viewport* viewport) = 0;

		//! Get current viewport
		virtual Viewport getViewport() = 0;

		//! Set scissors
		virtual void setScissors(float x, float y, float w, float h) = 0;

		//! Draw non indexed primitives
		virtual void draw(PrimitiveMode primitiveMode, size_t verticesStart, size_t verticesCount) = 0;

		//! Draw indexed primitives
		virtual void drawIndexed(PrimitiveMode primitiveMode, size_t indexStart, size_t indexCount, int baseVertexLocation) = 0;

	};

	extern IRenderDevice* g_renderDevice;
}

#endif // !GRAPHICS_CORE_DEVICE_H
