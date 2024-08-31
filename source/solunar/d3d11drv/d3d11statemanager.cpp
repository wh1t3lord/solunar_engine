#include "d3d11drv_pch.h"
#include "d3d11drv/d3d11statemanager.h"
#include "d3d11drv/d3d11device.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// #TODO LIST:
//  - Fix GetD3DBlend and GetD3DBlendOp, return assert because memset'inf BlendStateDesc
//  - Blend and BlendOp start from 1 like D3D11, remove it
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace solunar
{

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

D3D11StateManager::D3D11StateManager()
{
}

D3D11StateManager::~D3D11StateManager()
{
}

void D3D11StateManager::Init()
{
}

void D3D11StateManager::Shutdown()
{
    for (auto it : m_blendStates)
    {
        DestroyBlendState(it.second);
    }

    m_blendStates.clear();

    for (auto it : m_depthStencilStates)
    {
        DestroyDepthStencilState(it.second);
    }

    m_depthStencilStates.clear();

    for (auto it : m_samplerStates)
    {
        DestroySamplerState(it.second);
    }

    m_samplerStates.clear();

    for (auto it : m_rasterizerStates)
    {
        DestroyRasterizerState(it.second);
    }

    m_rasterizerStates.clear();
}

IRasterizerState* D3D11StateManager::CreateRasterizerState(const RasterizerStateDesc& rasterizerDesc)
{
    // Find already created IRasterizerState
	auto it = m_rasterizerStates.find(rasterizerDesc);
    if (it != m_rasterizerStates.end())
    {
        return (*it).second;
    }

	D3D11_RASTERIZER_DESC d3drasterizerDesc = {};
    d3drasterizerDesc.FillMode              = GetD3DFillMode(rasterizerDesc.m_fillMode);
    d3drasterizerDesc.CullMode              = GetD3DCullMode(rasterizerDesc.m_cullMode);
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

void D3D11StateManager::DestroyRasterizerState(IRasterizerState* rasterizerState)
{
    ID3D11RasterizerState* d3drasterizerState = (ID3D11RasterizerState*)rasterizerState;
    d3drasterizerState->Release();
}

void D3D11StateManager::SetRasterizerState(IRasterizerState* rasterizerState)
{
    ID3D11RasterizerState* d3drasterizerState = (ID3D11RasterizerState*)rasterizerState;
    g_d3d11Device->getDeviceContext()->RSSetState(d3drasterizerState);
}

ISamplerState* D3D11StateManager::CreateSamplerState(const SamplerDesc& samplerDesc)
{
    // Find already created ISamplerState
    auto it = m_samplerStates.find(samplerDesc);
    if (it != m_samplerStates.end())
    {
        return (*it).second;
    }

    ISamplerState* samplerState = g_renderDevice->CreateSamplerState(samplerDesc);

    m_samplerStates.emplace(samplerDesc, samplerState);

    return samplerState;
}

void D3D11StateManager::DestroySamplerState(ISamplerState* samplerState)
{
    // ???
    mem_delete(samplerState);
}

IBlendState* D3D11StateManager::CreateBlendState(const BlendStateDesc& blendStateDesc)
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
        blendDesc.RenderTarget[i].SrcBlend               = GetD3DBlend(blendStateDesc.m_renderTarget[i].m_srcBlend);
        blendDesc.RenderTarget[i].DestBlend              = GetD3DBlend(blendStateDesc.m_renderTarget[i].m_destBlend);
        blendDesc.RenderTarget[i].BlendOp                = GetD3DBlendOp(blendStateDesc.m_renderTarget[i].m_blendOp);
        blendDesc.RenderTarget[i].SrcBlendAlpha          = GetD3DBlend(blendStateDesc.m_renderTarget[i].m_srcBlendAlpha);
        blendDesc.RenderTarget[i].DestBlendAlpha         = GetD3DBlend(blendStateDesc.m_renderTarget[i].m_destBlendAlpha);
        blendDesc.RenderTarget[i].BlendOpAlpha           = GetD3DBlendOp(blendStateDesc.m_renderTarget[i].m_blendOpAlpha);
        blendDesc.RenderTarget[i].RenderTargetWriteMask  = blendStateDesc.m_renderTarget[i].m_renderTargetWriteMask;
    }

    // create d3d object
    IBlendState* blendState = nullptr;
    D3D11_CHECK(g_d3d11Device->getDevice()->CreateBlendState(&blendDesc, reinterpret_cast<ID3D11BlendState**>(&blendState)));

    m_blendStates.emplace(blendStateDesc, blendState);

    return blendState;
}

void D3D11StateManager::DestroyBlendState(IBlendState* blendState)
{
    ID3D11BlendState* d3dBlendState = (ID3D11BlendState*)blendState;
    d3dBlendState->Release();
}

void D3D11StateManager::SetBlendState(IBlendState* blendState, const float blendFactor[4], uint32_t sampleMask)
{
    ID3D11BlendState* d3dBlendState = (ID3D11BlendState*)blendState;
    g_d3d11Device->getDeviceContext()->OMSetBlendState(d3dBlendState, blendFactor, sampleMask);
}

IDepthStencilState* D3D11StateManager::CreateDepthStencilState(const DepthStencilDesc& desc)
{
    // Find already created IDepthStencilState
    auto it = m_depthStencilStates.find(desc);
    if (it != m_depthStencilStates.end())
    {
        return (*it).second;
    }

    D3D11_DEPTH_STENCIL_DESC dsDesc     = {};
    dsDesc.DepthEnable                  = desc.m_depthEnable;
    dsDesc.DepthWriteMask               = GetD3DDepthWriteMask(desc.m_depthWriteMask);
    dsDesc.DepthFunc                    = GetD3DComparsionFunc(desc.m_depthFunc);
    dsDesc.StencilEnable                = desc.m_stencilEnable;
    dsDesc.StencilReadMask              = desc.m_stencilReadMask;
    dsDesc.StencilWriteMask             = desc.m_stencilWriteMask;
    dsDesc.FrontFace.StencilFailOp      = GetD3DStencilOperation(desc.m_frontFace.m_stencilFailOp);
    dsDesc.FrontFace.StencilDepthFailOp = GetD3DStencilOperation(desc.m_frontFace.m_stencilDepthFailOp);
    dsDesc.FrontFace.StencilPassOp      = GetD3DStencilOperation(desc.m_frontFace.m_stencilPassOp);
    dsDesc.FrontFace.StencilFunc        = GetD3DComparsionFunc(desc.m_frontFace.m_stencilFunc);
    dsDesc.BackFace.StencilFailOp       = GetD3DStencilOperation(desc.m_backFace.m_stencilFailOp);
    dsDesc.BackFace.StencilDepthFailOp  = GetD3DStencilOperation(desc.m_backFace.m_stencilDepthFailOp);
    dsDesc.BackFace.StencilPassOp       = GetD3DStencilOperation(desc.m_backFace.m_stencilPassOp);
    dsDesc.BackFace.StencilFunc         = GetD3DComparsionFunc(desc.m_backFace.m_stencilFunc);

    // create d3d object
    IDepthStencilState* depthStencilState = nullptr;
    D3D11_CHECK(g_d3d11Device->getDevice()->CreateDepthStencilState(&dsDesc, reinterpret_cast<ID3D11DepthStencilState**>(&depthStencilState)));

    m_depthStencilStates.emplace(desc, depthStencilState);

    return depthStencilState;
}

void D3D11StateManager::DestroyDepthStencilState(IDepthStencilState* state)
{
    ID3D11DepthStencilState* d3dDSState = (ID3D11DepthStencilState*)state;
    d3dDSState->Release();
}

void D3D11StateManager::SetDepthStencilState(IDepthStencilState* state, uint32_t stencilRef)
{
    ID3D11DepthStencilState* d3dDSState = (ID3D11DepthStencilState*)state;
    g_d3d11Device->getDeviceContext()->OMSetDepthStencilState(d3dDSState, stencilRef);
}


}