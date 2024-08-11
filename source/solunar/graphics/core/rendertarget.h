#ifndef GRAPHICS_CORE_RENDERTARGET_H
#define GRAPHICS_CORE_RENDERTARGET_H

namespace solunar {

	class ITexture2D;

	const int kMaxRenderTargetTextures = 8;

	struct RenderTargetCreationDesc
	{
		ITexture2D* m_textures2D[kMaxRenderTargetTextures];
		ITexture2D* m_depthTexture2D;
		size_t m_textures2DCount;
	};

	//! Interface class for the render target object.
	class IRenderTarget
	{
	public:
		virtual ~IRenderTarget() {}

		//! Release the render target.
		virtual void Release() = 0;

		//! Set debug name.
		virtual void setDebugName(const char* debugName) = 0;
	};

}

#endif // !GRAPHICS_CORE_RENDERTARGET_H
