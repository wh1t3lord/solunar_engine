#include "d3d11drv_pch.h"
#include "d3d11drv/d3d11statemanager.h"
#include "d3d11drv/d3d11device.h"

namespace solunar
{

inline D3D11_FILL_MODE getD3DFillMode(FillMode fillmode)
{
    switch (fillmode)
    {
    case FillMode::Wireframe:
        return D3D11_FILL_WIREFRAME;
    case FillMode::Solid:
        return D3D11_FILL_SOLID;

    default:
        break;
    }

    return (D3D11_FILL_MODE)0;
}

inline D3D11_CULL_MODE getD3DCullMode(CullMode cullmode)
{
    switch (cullmode)
    {
    case CullMode::None:
        return D3D11_CULL_NONE;
    case CullMode::Front:
        return D3D11_CULL_FRONT;
    case CullMode::Back:
        return D3D11_CULL_BACK;
  
    default:
        break;
    }

    return (D3D11_CULL_MODE)0;
}

inline D3D11_BLEND getD3DBlend(Blend blend)
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

    return (D3D11_BLEND)blend;
}

inline D3D11_BLEND_OP getD3DBlendOp(BlendOp blendop)
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

    return (D3D11_BLEND_OP)0;
}

D3D11StateManager::D3D11StateManager()
{
}

D3D11StateManager::~D3D11StateManager()
{
}

void D3D11StateManager::init()
{
}

void D3D11StateManager::shutdown()
{
    for (auto it : m_blendStates)
    {
        destroyBlendState(it.second);
    }

    m_blendStates.clear();

    for (auto it : m_depthStencilStates)
    {
        destroyDepthStencilState(it.second);
    }

    m_depthStencilStates.clear();

    for (auto it : m_samplerStates)
    {
        destroySamplerState(it.second);
    }

    m_samplerStates.clear();

    for (auto it : m_rasterizerStates)
    {
        destroyRasterizerState(it.second);
    }

    m_rasterizerStates.clear();
}

IRasterizerState* D3D11StateManager::createRasterizerState(const RasterizerStateDesc& rasterizerDesc)
{
    // Find already created IRasterizerState
	auto it = m_rasterizerStates.find(rasterizerDesc);
    if (it != m_rasterizerStates.end())
    {
        return (*it).second;
    }

	D3D11_RASTERIZER_DESC d3drasterizerDesc = {};
    d3drasterizerDesc.FillMode              = getD3DFillMode(rasterizerDesc.m_fillMode);
    d3drasterizerDesc.CullMode              = getD3DCullMode(rasterizerDesc.m_cullMode);
    d3drasterizerDesc.FrontCounterClockwise = rasterizerDesc.m_frontCCW;
    d3drasterizerDesc.DepthBias             = rasterizerDesc.m_depthBias;
    d3drasterizerDesc.DepthBiasClamp        = rasterizerDesc.m_depthBiasClamp;
    d3drasterizerDesc.SlopeScaledDepthBias  = rasterizerDesc.m_slopeScaledDepthBias;
    d3drasterizerDesc.DepthClipEnable       = rasterizerDesc.m_depthClipEnable;
    d3drasterizerDesc.ScissorEnable         = rasterizerDesc.m_scissorEnable;
    d3drasterizerDesc.MultisampleEnable     = rasterizerDesc.m_multisampleEnable;
    d3drasterizerDesc.AntialiasedLineEnable = rasterizerDesc.m_antialiasedLineEnable;

    // create d3d object
    IRasterizerState* rasterizerState = nullptr;
    D3D11_CHECK(g_d3d11Device->getDevice()->CreateRasterizerState(&d3drasterizerDesc, reinterpret_cast<ID3D11RasterizerState**>(&rasterizerState)));

    m_rasterizerStates.emplace(rasterizerDesc, rasterizerState);

	return rasterizerState;
}

void D3D11StateManager::destroyRasterizerState(IRasterizerState* rasterizerState)
{
    ID3D11RasterizerState* d3drasterizerState = (ID3D11RasterizerState*)rasterizerState;
    d3drasterizerState->Release();
}

void D3D11StateManager::setRasterizerState(IRasterizerState* rasterizerState)
{
    ID3D11RasterizerState* d3drasterizerState = (ID3D11RasterizerState*)rasterizerState;
    g_d3d11Device->getDeviceContext()->RSSetState(d3drasterizerState);
}

ISamplerState* D3D11StateManager::createSamplerState(const SamplerDesc& samplerDesc)
{
    // Find already created ISamplerState
    auto it = m_samplerStates.find(samplerDesc);
    if (it != m_samplerStates.end())
    {
        return (*it).second;
    }

    ISamplerState* samplerState = g_renderDevice->createSamplerState(samplerDesc);

    m_samplerStates.emplace(samplerDesc, samplerState);

    return samplerState;
}

void D3D11StateManager::destroySamplerState(ISamplerState* samplerState)
{
    // ???
    mem_delete(samplerState);
}

IBlendState* D3D11StateManager::createBlendState(const BlendStateDesc& blendStateDesc)
{
    // Find already created IBlendState
    auto it = m_blendStates.find(blendStateDesc);
    if (it != m_blendStates.end())
    {
        return (*it).second;
    }

    D3D11_BLEND_DESC blendDesc          = {};
    blendDesc.AlphaToCoverageEnable     = blendStateDesc.m_alphaToCoverageEnable;
    blendDesc.IndependentBlendEnable    = blendStateDesc.m_independentBlendEnable;
    for (int i = 0; i < 8; i++)
    {
        blendDesc.RenderTarget[i].BlendEnable            = blendStateDesc.m_renderTarget[i].m_blendEnable;
        blendDesc.RenderTarget[i].SrcBlend               = getD3DBlend(blendStateDesc.m_renderTarget[i].m_srcBlend);
        blendDesc.RenderTarget[i].DestBlend              = getD3DBlend(blendStateDesc.m_renderTarget[i].m_destBlend);
        blendDesc.RenderTarget[i].BlendOp                = getD3DBlendOp(blendStateDesc.m_renderTarget[i].m_blendOp);
        blendDesc.RenderTarget[i].SrcBlendAlpha          = getD3DBlend(blendStateDesc.m_renderTarget[i].m_srcBlendAlpha);
        blendDesc.RenderTarget[i].DestBlendAlpha         = getD3DBlend(blendStateDesc.m_renderTarget[i].m_destBlendAlpha);
        blendDesc.RenderTarget[i].BlendOpAlpha           = getD3DBlendOp(blendStateDesc.m_renderTarget[i].m_blendOpAlpha);
        blendDesc.RenderTarget[i].RenderTargetWriteMask  = blendStateDesc.m_renderTarget[i].m_renderTargetWriteMask;
    }

    // create d3d object
    IBlendState* blendState = nullptr;
    D3D11_CHECK(g_d3d11Device->getDevice()->CreateBlendState(&blendDesc, reinterpret_cast<ID3D11BlendState**>(&blendState)));

    m_blendStates.emplace(blendStateDesc, blendState);

    return blendState;
}

void D3D11StateManager::destroyBlendState(IBlendState* blendState)
{
    ID3D11BlendState* d3dBlendState = (ID3D11BlendState*)blendState;
    d3dBlendState->Release();
}

void D3D11StateManager::setBlendState(IBlendState* blendState, const float blendFactor[4], uint32_t sampleMask)
{
    ID3D11BlendState* d3dBlendState = (ID3D11BlendState*)blendState;
    g_d3d11Device->getDeviceContext()->OMSetBlendState(d3dBlendState, blendFactor, sampleMask);
}

IDepthStencilState* D3D11StateManager::createDepthStencilState(const DepthStencilDesc& desc)
{
    // Find already created IDepthStencilState
    auto it = m_depthStencilStates.find(desc);
    if (it != m_depthStencilStates.end())
    {
        return (*it).second;
    }

    D3D11_DEPTH_STENCIL_DESC dsDesc     = {};
    dsDesc.DepthEnable                  = desc.m_depthEnable;
    dsDesc.DepthWriteMask               = (D3D11_DEPTH_WRITE_MASK)desc.m_depthWriteMask;
    dsDesc.DepthFunc                    = (D3D11_COMPARISON_FUNC)desc.m_depthFunc;
    dsDesc.StencilEnable                = desc.m_stencilEnable;
    dsDesc.StencilReadMask              = desc.m_stencilReadMask;
    dsDesc.StencilWriteMask             = desc.m_stencilWriteMask;
    dsDesc.FrontFace.StencilFailOp      = (D3D11_STENCIL_OP)desc.m_frontFace.m_stencilFailOp;
    dsDesc.FrontFace.StencilDepthFailOp = (D3D11_STENCIL_OP)desc.m_frontFace.m_stencilDepthFailOp;
    dsDesc.FrontFace.StencilPassOp      = (D3D11_STENCIL_OP)desc.m_frontFace.m_stencilPassOp;
    dsDesc.FrontFace.StencilFunc        = (D3D11_COMPARISON_FUNC)desc.m_frontFace.m_stencilFunc;
    dsDesc.BackFace.StencilFailOp       = (D3D11_STENCIL_OP)desc.m_backFace.m_stencilFailOp;
    dsDesc.BackFace.StencilDepthFailOp  = (D3D11_STENCIL_OP)desc.m_backFace.m_stencilDepthFailOp;
    dsDesc.BackFace.StencilPassOp       = (D3D11_STENCIL_OP)desc.m_backFace.m_stencilPassOp;
    dsDesc.BackFace.StencilFunc         = (D3D11_COMPARISON_FUNC)desc.m_backFace.m_stencilFunc;

    // create d3d object
    IDepthStencilState* depthStencilState = nullptr;
    D3D11_CHECK(g_d3d11Device->getDevice()->CreateDepthStencilState(&dsDesc, reinterpret_cast<ID3D11DepthStencilState**>(&depthStencilState)));

    m_depthStencilStates.emplace(desc, depthStencilState);

    return depthStencilState;
}

void D3D11StateManager::destroyDepthStencilState(IDepthStencilState* state)
{
    ID3D11DepthStencilState* d3dDSState = (ID3D11DepthStencilState*)state;
    d3dDSState->Release();
}

void D3D11StateManager::setDepthStencilState(IDepthStencilState* state, uint32_t stencilRef)
{
    ID3D11DepthStencilState* d3dDSState = (ID3D11DepthStencilState*)state;
    g_d3d11Device->getDeviceContext()->OMSetDepthStencilState(d3dDSState, stencilRef);
}


}