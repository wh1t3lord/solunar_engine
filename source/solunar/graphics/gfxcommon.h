#ifndef GFXCOMMON_H
#define GFXCOMMON_H

namespace engine
{
	enum class ImageFormat
	{
		// Unsigned byte 
		RGB16,
		RGBA16,
		RGB32,
		RGBA32,
		DEPTH24,
		DEPTH32,

		// Float

		// Single component
		R32,
		R32F,

		RGB16F,
		RGBA16F,
		RGB32F,
		RGBA32F,
		DEPTH32F
	};

	enum class BufferMapping
	{
		ReadOnly,
		WriteOnly,
		ReadAndWrite
	};

	enum class BufferDataType
	{
		UnsignedInt,
	};

	enum class BufferAccess
	{
		Static,
		Dynamic,
		Stream
	};

	enum class FillMode
	{
		Wireframe,
		Solid,
		Count
	};

	enum class CullMode
	{
		None,
		Front,
		Back,
		Count
	};

	struct BufferCreationDesc
	{
		void* m_data;
		size_t m_dataSize;
		BufferAccess m_access;
	};

	enum class BufferType : uint16_t
	{
		VertexBuffer,
		IndexBuffer,
		ConstantBuffer
	};

	struct BufferDesc
	{
		BufferType m_bufferType;
		BufferAccess m_bufferAccess;
		uint32_t m_bufferMemorySize;
	};

	struct SubresourceDesc
	{
		void* m_memory;
		uint32_t m_memoryPitch;
		uint32_t m_memorySlicePitch;
	};

	struct IBufferBase
	{
		virtual ~IBufferBase() {}

		virtual void* map(BufferMapping mapping) = 0;
		virtual void  unmap() = 0;

		virtual void updateSubresource(void* data, size_t size) = 0;

		virtual BufferDesc getBufferDesc() = 0;
	};

	struct ShaderCreationDesc
	{
		const char* m_byteCode;
		size_t m_byteCodeSize;
		bool m_isHasPermutations;
		bool m_isPixel;
	};

	enum PrimitiveMode
	{
		PM_LineList,
		PM_LineStrip,
		PM_TriangleList,
		PM_TriangleStrip,

		PM_UNUSED_COUNT
	};

	struct Viewport
	{
		int m_x;
		int m_y;
		int m_width;
		int m_height;
	};

	enum class TextureWrapComponent
	{
		S,
		T
	};

	enum class TextureWrap
	{
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	enum class TextureFilter
	{
		Nearest,
		Linear,
		NearestMipmapNearest,
		LinearMipmapNearest,
		NearestMipmapLinear,
		LinearMipmapLinear
	};

	enum class TextureType
	{
		Texture1D,
		Texture2D,
		TextureCubemap
	};

	struct TextureDesc
	{
		TextureType m_textureType;
		uint32_t m_width = 1;
		uint32_t m_height = 1;
		uint32_t m_mipmapLevel = 0;
		ImageFormat m_format;
	};

	struct SurfaceDesc
	{
		uint32_t m_width;
		uint32_t m_height;
	};

	struct SamplerDesc
	{
		TextureFilter m_minFilter;
		TextureFilter m_magFilter;
		TextureWrap m_wrapS;
		TextureWrap m_wrapT;
		TextureWrap m_wrapRepeat;
		float m_anisotropyLevel;
	};

	struct RasterizerStateDesc
	{
		FillMode m_fillMode;
		CullMode m_cullMode;
		bool m_frontCCW;
		int m_depthBias;
		float m_depthBiasClamp;
		float m_slopeScaledDepthBias;
		bool m_depthClipEnable;
		bool m_scissorEnable;
		bool m_multisampleEnable;
		bool m_antialiasedLineEnable;
	};
}

#endif // GFXCOMMON_H
