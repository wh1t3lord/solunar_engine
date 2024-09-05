#ifndef D3D11UTILS_H
#define D3D11UTILS_H

namespace solunar
{

bool d3d11ErrorBackend(HRESULT hr, const char* expression, const char* filename, int line, const char* function);

inline UINT GetD3D11BindFlags(const BufferDesc& bufferDesc)
{
    switch (bufferDesc.m_bufferType)
    {
    case BufferType::VertexBuffer:
        return D3D11_BIND_VERTEX_BUFFER;

    case BufferType::IndexBuffer:
        return D3D11_BIND_INDEX_BUFFER;

    case BufferType::ConstantBuffer:
        return D3D11_BIND_CONSTANT_BUFFER;

    default:
        break;
    }

    Assert2(0, "Unknowed buffer type");
    return 0;
}

inline D3D11_MAP GetD3D11Map(BufferMapping mapping)
{
    switch (mapping)
    {
    case BufferMapping::ReadOnly:
        return D3D11_MAP_READ;

    case BufferMapping::WriteOnly:
        return D3D11_MAP_WRITE_DISCARD;

    case BufferMapping::ReadAndWrite:
        return D3D11_MAP_READ_WRITE;

    default:
        break;
    }

    Assert2(0, "Unknowed buffer mapping type");
    return (D3D11_MAP)0;
}

inline D3D11_USAGE GetD3D11Usage(const BufferDesc& desc)
{
    switch (desc.m_bufferAccess)
    {
    case BufferAccess::Static:
        return D3D11_USAGE_DEFAULT;

    case BufferAccess::Dynamic:
    case BufferAccess::Stream:
        return D3D11_USAGE_DYNAMIC;

    default:
        break;
    }

    Assert2(0, "Unknowed buffer access type");
    return (D3D11_USAGE)0;
}

inline D3D11_FILL_MODE GetD3DFillMode(FillMode fillmode)
{
    switch (fillmode)
    {
    case FillMode::Wireframe:
        return D3D11_FILL_WIREFRAME;
    case FillMode::Solid:
        return D3D11_FILL_SOLID;
    }

    Assert2(0, "Unknowed argument");
    return (D3D11_FILL_MODE)0;
}

inline D3D11_CULL_MODE GetD3DCullMode(CullMode cullmode)
{
    switch (cullmode)
    {
    case CullMode::None:
        return D3D11_CULL_NONE;
    case CullMode::Front:
        return D3D11_CULL_FRONT;
    case CullMode::Back:
        return D3D11_CULL_BACK;
    }

    Assert2(0, "Unknowed argument");
    return (D3D11_CULL_MODE)0;
}

inline D3D11_PRIMITIVE_TOPOLOGY GetD3D11PrimitiveTopology(PrimitiveMode primitiveMode)
{
    switch (primitiveMode)
    {
    case PM_LineList:
        return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

    case PM_LineStrip:
        return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

    case PM_TriangleList:
        return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    case PM_TriangleStrip:
        return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

    default:
        break;
    }

    Assert2(0, "Unknowed or unsupported primitive mode");
    return (D3D11_PRIMITIVE_TOPOLOGY)0;
}

inline D3D11_BLEND GetD3DBlend(Blend blend)
{
    switch (blend)
    {
    case BLEND_ZERO:
        return D3D11_BLEND_ZERO;
    case BLEND_ONE:
        return D3D11_BLEND_ONE;
    case BLEND_SRC_COLOR:
        return D3D11_BLEND_SRC_COLOR;
    case BLEND_INV_SRC_COLOR:
        return D3D11_BLEND_INV_SRC_COLOR;
    case BLEND_SRC_ALPHA:
        return D3D11_BLEND_SRC_ALPHA;
    case BLEND_INV_SRC_ALPHA:
        return D3D11_BLEND_INV_SRC_ALPHA;
    case BLEND_DEST_ALPHA:
        return D3D11_BLEND_DEST_ALPHA;
    case BLEND_INV_DEST_ALPHA:
        return D3D11_BLEND_INV_DEST_ALPHA;
    case BLEND_DEST_COLOR:
        return D3D11_BLEND_DEST_COLOR;
    case BLEND_INV_DEST_COLOR:
        return D3D11_BLEND_INV_DEST_COLOR;
    case BLEND_SRC_ALPHA_SAT:
        return D3D11_BLEND_SRC_ALPHA_SAT;
    case BLEND_BLEND_FACTOR:
        return D3D11_BLEND_BLEND_FACTOR;
    case BLEND_INV_BLEND_FACTOR:
        return D3D11_BLEND_INV_BLEND_FACTOR;
    case BLEND_SRC1_COLOR:
        return D3D11_BLEND_SRC1_COLOR;
    case BLEND_INV_SRC1_COLOR:
        return D3D11_BLEND_INV_SRC1_COLOR;
    case BLEND_SRC1_ALPHA:
        return D3D11_BLEND_SRC1_ALPHA;
    case BLEND_INV_SRC1_ALPHA:
        return D3D11_BLEND_INV_SRC1_ALPHA;
    }

    //Assert2(0, "Unknowed argument");
    return (D3D11_BLEND)blend;
}

inline D3D11_BLEND_OP GetD3DBlendOp(BlendOp blendop)
{

    switch (blendop)
    {
    case BLEND_OP_ADD:
        return D3D11_BLEND_OP_ADD;
    case BLEND_OP_SUBTRACT:
        return D3D11_BLEND_OP_SUBTRACT;
    case BLEND_OP_REV_SUBTRACT:
        return D3D11_BLEND_OP_REV_SUBTRACT;
    case BLEND_OP_MIN:
        return D3D11_BLEND_OP_MIN;
    case BLEND_OP_MAX:
        return D3D11_BLEND_OP_MAX;
    }

    //Assert2(0, "Unknowed argument");
    return (D3D11_BLEND_OP)0;
}

inline D3D11_DEPTH_WRITE_MASK GetD3DDepthWriteMask(DepthWriteMask mask)
{
    switch (mask)
    {
    case DEPTH_WRITE_MASK_ZERO:
        return D3D11_DEPTH_WRITE_MASK_ZERO;
    case DEPTH_WRITE_MASK_ALL:
        return D3D11_DEPTH_WRITE_MASK_ALL;
    }

    Assert2(0, "Unknowed argument");
    return (D3D11_DEPTH_WRITE_MASK)0;
}

inline D3D11_COMPARISON_FUNC GetD3DComparsionFunc(ComparsionFunction func)
{
    switch (func)
    {
    case COMPARISON_NEVER:
        return D3D11_COMPARISON_NEVER;
    case COMPARISON_LESS:
        return D3D11_COMPARISON_LESS;
    case COMPARISON_EQUAL:
        return D3D11_COMPARISON_EQUAL;
    case COMPARISON_LESS_EQUAL:
        return D3D11_COMPARISON_LESS_EQUAL;
    case COMPARISON_GREATER:
        return D3D11_COMPARISON_GREATER;
    case COMPARISON_NOT_EQUAL:
        return D3D11_COMPARISON_NOT_EQUAL;
    case COMPARISON_GREATER_EQUAL:
        return D3D11_COMPARISON_GREATER_EQUAL;
    case COMPARISON_ALWAYS:
        return D3D11_COMPARISON_ALWAYS;
    }

    Assert2(0, "Unknowed argument");
    return (D3D11_COMPARISON_FUNC)0;
}

inline D3D11_STENCIL_OP GetD3DStencilOperation(StencilOperation operation)
{
    switch (operation)
    {
    case STENCIL_OP_KEEP:
        return D3D11_STENCIL_OP_KEEP;
    case STENCIL_OP_ZERO:
        return D3D11_STENCIL_OP_ZERO;
    case STENCIL_OP_REPLACE:
        return D3D11_STENCIL_OP_REPLACE;
    case STENCIL_OP_INCR_SAT:
        return D3D11_STENCIL_OP_INCR_SAT;
    case STENCIL_OP_DECR_SAT:
        return D3D11_STENCIL_OP_DECR_SAT;
    case STENCIL_OP_INVERT:
        return D3D11_STENCIL_OP_INVERT;
    case STENCIL_OP_INCR:
        return D3D11_STENCIL_OP_INCR;
    case STENCIL_OP_DECR:
        return D3D11_STENCIL_OP_DECR;
    }

    Assert2(0, "Unknowed argument");
    return (D3D11_STENCIL_OP)0;
}

inline DXGI_FORMAT GetDXGIFormat(ImageFormat format)
{
    switch (format)
    {
    case ImageFormat::RGBA16F:
        return DXGI_FORMAT_R16G16B16A16_FLOAT;

    case ImageFormat::DEPTH24:
        //	return DXGI_FORMAT_D24_UNORM_S8_UINT;
        return DXGI_FORMAT_R24G8_TYPELESS;

    case ImageFormat::DEPTH32:
    case ImageFormat::DEPTH32F:
        return DXGI_FORMAT_D32_FLOAT;

    case ImageFormat::R32:
        return DXGI_FORMAT_R8_UNORM;

    case ImageFormat::RGB32:
        return DXGI_FORMAT_R8G8B8A8_UNORM;

    case ImageFormat::RGBA32:
        return DXGI_FORMAT_R8G8B8A8_UNORM;

    case ImageFormat::RGB32F:
        return DXGI_FORMAT_R32G32B32_FLOAT;

    case ImageFormat::RGBA32F:
        return DXGI_FORMAT_R32G32B32A32_FLOAT;

    case ImageFormat::RGBA32_UNORM:
        return DXGI_FORMAT_R8G8B8A8_UNORM;

    case ImageFormat::RG32F:
        return DXGI_FORMAT_R32G32_FLOAT;

    default:
        break;
    }

    Assert2(0, "Unknowed image format passed");

    return DXGI_FORMAT_UNKNOWN;
}

inline DXGI_FORMAT ConvertToDepthStencilFormat(DXGI_FORMAT nFormat)
{
    switch (nFormat)
    {
    case DXGI_FORMAT_R24G8_TYPELESS:
        return DXGI_FORMAT_D24_UNORM_S8_UINT;
    case DXGI_FORMAT_R32G8X24_TYPELESS:
        return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        //	case DXGI_FORMAT_R32G32_TYPELESS:   
        //		return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    case DXGI_FORMAT_R32_TYPELESS:
        return DXGI_FORMAT_D32_FLOAT;
    case DXGI_FORMAT_R16_TYPELESS:
        return DXGI_FORMAT_D16_UNORM;
    case DXGI_FORMAT_D32_FLOAT:
        return DXGI_FORMAT_D32_FLOAT; // bypass
    default:
        //ASSERT(false);
        return nFormat;
        break;
    }
}

inline DXGI_FORMAT ConvertToShaderResourceFmt(DXGI_FORMAT nFormat)
{
    //handle special cases
    switch (nFormat)
    {
    case DXGI_FORMAT_R24G8_TYPELESS:
        return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    case DXGI_FORMAT_R32G8X24_TYPELESS:
        return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
        //	case DXGI_FORMAT_R32G32_TYPELESS:   
        //		return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    case DXGI_FORMAT_R32_TYPELESS:
        return DXGI_FORMAT_R32_FLOAT;
    case DXGI_FORMAT_R16_TYPELESS:
        return DXGI_FORMAT_R16_UNORM;
    default:
        return nFormat;
        //pass through		
    }
}

inline DXGI_FORMAT ConvertToTypelessFmt(DXGI_FORMAT fmt)
{
    switch (fmt)
    {
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
    case DXGI_FORMAT_R32G32B32A32_UINT:
    case DXGI_FORMAT_R32G32B32A32_SINT:
        return DXGI_FORMAT_R32G32B32A32_TYPELESS;
        break;

    case DXGI_FORMAT_R32G32B32_FLOAT:
    case DXGI_FORMAT_R32G32B32_UINT:
    case DXGI_FORMAT_R32G32B32_SINT:
        return DXGI_FORMAT_R32G32B32_TYPELESS;
        break;

    case DXGI_FORMAT_R16G16B16A16_FLOAT:
    case DXGI_FORMAT_R16G16B16A16_UNORM:
    case DXGI_FORMAT_R16G16B16A16_UINT:
    case DXGI_FORMAT_R16G16B16A16_SNORM:
    case DXGI_FORMAT_R16G16B16A16_SINT:
        return DXGI_FORMAT_R16G16B16A16_TYPELESS;
        break;

    case DXGI_FORMAT_R32G32_FLOAT:
    case DXGI_FORMAT_R32G32_UINT:
    case DXGI_FORMAT_R32G32_SINT:
        return DXGI_FORMAT_R32G32_TYPELESS;
        break;

    case DXGI_FORMAT_R10G10B10A2_UNORM:
    case DXGI_FORMAT_R10G10B10A2_UINT:
        return DXGI_FORMAT_R10G10B10A2_TYPELESS;
        break;

    case DXGI_FORMAT_R8G8B8A8_UNORM:
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    case DXGI_FORMAT_R8G8B8A8_UINT:
    case DXGI_FORMAT_R8G8B8A8_SNORM:
    case DXGI_FORMAT_R8G8B8A8_SINT:
        return DXGI_FORMAT_R8G8B8A8_TYPELESS;
        break;

    case DXGI_FORMAT_BC1_UNORM:
    case DXGI_FORMAT_BC1_UNORM_SRGB:
        return DXGI_FORMAT_BC1_TYPELESS;
        break;

    case DXGI_FORMAT_BC2_UNORM:
    case DXGI_FORMAT_BC2_UNORM_SRGB:
        return DXGI_FORMAT_BC2_TYPELESS;
        break;

    case DXGI_FORMAT_BC3_UNORM:
    case DXGI_FORMAT_BC3_UNORM_SRGB:
        return DXGI_FORMAT_BC3_TYPELESS;
        break;

        // todo: add missing formats if they found required

    default:
        //ASSERT(false);
        return fmt;
    }
}

}

#define D3D11_CHECK( STUFF )	\
	{							\
		HRESULT hr = STUFF;		\
		if (FAILED(hr))			\
			d3d11ErrorBackend(hr, #STUFF, __FILE__, __LINE__, __FUNCTION__);	\
	}

#endif // !D3D11UTILS_H
