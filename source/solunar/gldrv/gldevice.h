#ifndef GRAPHICS_GL_GLDEVICE_H
#define GRAPHICS_GL_GLDEVICE_H

#include "graphics/core/device.h"

namespace engine {

class GLDevice : public IRenderDevice
{
public:
	GLDevice();
	~GLDevice();

	void create();
	void destroy();

	//! Create render target.
	IRenderTarget* createRenderTarget(const RenderTargetCreationDesc& renderTargetDesc) override;

	//! Create buffer (vertex, index, constant and e.x.)
	IBufferBase* createBuffer(const BufferDesc& bufferDesc, const SubresourceDesc& subresourceDesc) override;

	//! Create texture 2d.
	ITexture2D* createTexture2D(const TextureDesc& textureDesc, const SubresourceDesc& subresourceDesc) override;
	
	//! Create texture sampler state.
	ISamplerState* createSamplerState(const SamplerDesc& samplerDesc) override;

	//! Set render target
	void setRenderTarget(IRenderTarget* rt) override;

	//! Set constant buffer.
	void setConstantBuffer(IBufferBase* cb) override;
	void setConstantBufferIndex(int slot, IBufferBase* cb) override;

	//! Set vertex buffer.
	void setVertexBuffer(IBufferBase* buffer, uint32_t stride, uint32_t offset) override;

	//! Set Index buffer.
	void setIndexBuffer(IBufferBase* buffer, bool use16bitsIndices) override;

	//! Set texture 2d.
	void setTexture2D(int slot, ITexture2D* texture) override;

	//! set sampler desc.
	void setSampler(int slot, ISamplerState* sampler) override;

	//! Set vertex format.
	void setVertexFormat(VertexFormat* vertex_format) override;

	//! Set viewport
	void setViewport(Viewport* viewport) override;

	//! Get current viewport
	Viewport getViewport() override;

	//! Set scissors
	void setScissors(float x, float y, float w, float h) override;

	//! Draw non indexed primitives
	void draw(PrimitiveMode primitiveMode, size_t verticesStart, size_t verticesCount) override;

	//! Draw indexed primitives
	void drawIndexed(PrimitiveMode primitiveMode, size_t indexStart, size_t indexCount, int baseVertexLocation) override;

private:
	Viewport m_viewport;

};

}

#endif // !GRAPHICS_GL_GLDEVICE_H
