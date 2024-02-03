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
		RG32F,
		RGB32F,
		RGBA32F,
		DEPTH32F,

		RGBA32_UNORM
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
		bool m_renderTargetUsage;
	};

	struct SurfaceDesc
	{
		uint32_t m_width;
		uint32_t m_height;
		ImageFormat m_format;
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

	enum InputAligment
	{
		INPUT_PER_VERTEX_DATA = 0,
		INPUT_PER_INSTANCE_DATA = 1
	};

	struct InputLayoutDesc
	{
		const char* m_semanticName;
		uint32_t m_semanticIndex;
		ImageFormat m_format;
		uint32_t m_inputSlot;
		uint32_t m_alignedByteOffset;
		InputAligment m_inputSlotClass;
		uint32_t m_instanceDataStepRate;
	};

	enum Blend
	{
		BLEND_ZERO = 1,
		BLEND_ONE = 2,
		BLEND_SRC_COLOR = 3,
		BLEND_INV_SRC_COLOR = 4,
		BLEND_SRC_ALPHA = 5,
		BLEND_INV_SRC_ALPHA = 6,
		BLEND_DEST_ALPHA = 7,
		BLEND_INV_DEST_ALPHA = 8,
		BLEND_DEST_COLOR = 9,
		BLEND_INV_DEST_COLOR = 10,
		BLEND_SRC_ALPHA_SAT = 11,
		BLEND_BLEND_FACTOR = 14,
		BLEND_INV_BLEND_FACTOR = 15,
		BLEND_SRC1_COLOR = 16,
		BLEND_INV_SRC1_COLOR = 17,
		BLEND_SRC1_ALPHA = 18,
		BLEND_INV_SRC1_ALPHA = 19
	};

	enum BlendOp
	{
		BLEND_OP_ADD = 1,
		BLEND_OP_SUBTRACT = 2,
		BLEND_OP_REV_SUBTRACT = 3,
		BLEND_OP_MIN = 4,
		BLEND_OP_MAX = 5
	};

	enum ColorWriteEnable
	{
		COLOR_WRITE_ENABLE_RED = 1,
		COLOR_WRITE_ENABLE_GREEN = 2,
		COLOR_WRITE_ENABLE_BLUE = 4,
		COLOR_WRITE_ENABLE_ALPHA = 8,
		COLOR_WRITE_ENABLE_ALL = (((COLOR_WRITE_ENABLE_RED | COLOR_WRITE_ENABLE_GREEN) | COLOR_WRITE_ENABLE_BLUE) | COLOR_WRITE_ENABLE_ALPHA)
	};

	struct RenderTargetBlendDesc
	{
		bool m_blendEnable;
		Blend m_srcBlend;
		Blend m_destBlend;
		BlendOp m_blendOp;
		Blend m_srcBlendAlpha;
		Blend m_destBlendAlpha;
		BlendOp m_blendOpAlpha;
		uint8_t m_renderTargetWriteMask;
	};

	struct BlendStateDesc
	{
		bool m_alphaToCoverageEnable;
		bool m_independentBlendEnable;
		RenderTargetBlendDesc m_renderTarget[8];
	};

	enum ComparsionFunction
	{
		COMPARISON_NEVER = 1,
		COMPARISON_LESS = 2,
		COMPARISON_EQUAL = 3,
		COMPARISON_LESS_EQUAL = 4,
		COMPARISON_GREATER = 5,
		COMPARISON_NOT_EQUAL = 6,
		COMPARISON_GREATER_EQUAL = 7,
		COMPARISON_ALWAYS = 8
	};

	enum DepthWriteMask
	{
		DEPTH_WRITE_MASK_ZERO = 0,
		DEPTH_WRITE_MASK_ALL = 1
	};

	enum StencilOperation
	{
		STENCIL_OP_KEEP = 1,
		STENCIL_OP_ZERO = 2,
		STENCIL_OP_REPLACE = 3,
		STENCIL_OP_INCR_SAT = 4,
		STENCIL_OP_DECR_SAT = 5,
		STENCIL_OP_INVERT = 6,
		STENCIL_OP_INCR = 7,
		STENCIL_OP_DECR = 8
	};

	struct DepthStencilOperationDesc
	{
		StencilOperation m_stencilFailOp;
		StencilOperation m_stencilDepthFailOp;
		StencilOperation m_stencilPassOp;
		ComparsionFunction m_stencilFunc;
	};

	struct DepthStencilDesc
	{
		bool m_depthEnable;
		DepthWriteMask m_depthWriteMask;
		ComparsionFunction m_depthFunc;
		bool m_stencilEnable;
		uint8_t m_stencilReadMask;
		uint8_t m_stencilWriteMask;
		DepthStencilOperationDesc m_frontFace;
		DepthStencilOperationDesc m_backFace;
	};
}

#endif // GFXCOMMON_H
