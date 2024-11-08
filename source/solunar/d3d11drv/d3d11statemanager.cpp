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
    D3D11_CHECK(g_d3d11Device->GetDevice()->CreateRasterizerState(&d3drasterizerDesc, reinterpret_cast<ID3D11RasterizerState**>(&rasterizerState)));

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
    g_d3d11Device->GetDeviceContext()->RSSetState(d3drasterizerState);
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
    D3D11_CHECK(g_d3d11Device->GetDevice()->CreateBlendState(&blendDesc, reinterpret_cast<ID3D11BlendState**>(&blendState)));

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
    g_d3d11Device->GetDeviceContext()->OMSetBlendState(d3dBlendState, blendFactor, sampleMask);
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
    D3D11_CHECK(g_d3d11Device->GetDevice()->CreateDepthStencilState(&dsDesc, reinterpret_cast<ID3D11DepthStencilState**>(&depthStencilState)));

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
    g_d3d11Device->GetDeviceContext()->OMSetDepthStencilState(d3dDSState, stencilRef);
}


}