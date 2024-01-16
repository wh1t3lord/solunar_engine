#include "d3d11drv_pch.h"
#include "d3d11drv/d3d11statemanager.h"

namespace engine
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

D3D11StateManager::D3D11StateManager()
{
}

D3D11StateManager::~D3D11StateManager()
{
}

IRasterizerState* D3D11StateManager::createRasterizerState(const RasterizerStateDesc& rasterizerDesc)
{
	//auto it = m_rasterizerStates.find(rasterizerDesc);

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
    D3D11_CHECK(g_d3d11Device->getD3DDevice()->CreateRasterizerState(&d3drasterizerDesc, reinterpret_cast<ID3D11RasterizerState**>(&rasterizerState)));

    m_rasterizerStates.emplace(rasterizerDesc, rasterizerState);

	return rasterizerState;
}

}