#include "gldrv_pch.h"
#include "gldrv/glstatemanager.h"
#include "gldrv/gldevice.h"

namespace engine
{

inline GLenum getGLFillMode(FillMode fillmode)
{
    switch (fillmode)
    {
    case FillMode::Wireframe:
        return GL_LINES;
    case FillMode::Solid:
        return  GL_FILL;

    default:
        break;
    }

    return (GLenum)0;
}

inline GLenum getGLCullMode(CullMode cullmode)
{
    switch (cullmode)
    {
    case CullMode::None:
        return GL_FRONT_AND_BACK;
    case CullMode::Front:
        return  GL_FRONT;
    case CullMode::Back:
        return GL_BACK;
  
    default:
        break;
    }

    return (GLenum)0;
}

inline GLenum getGLBlend(Blend blend)
{
    switch (blend)
    {
    case BLEND_ZERO:
        return GL_BLEND_COLOR;
    case BLEND_ONE:
        break;
    case BLEND_SRC_COLOR:
        break;
    case BLEND_INV_SRC_COLOR:
        break;
    case BLEND_SRC_ALPHA:
        break;
    case BLEND_INV_SRC_ALPHA:
        break;
    case BLEND_DEST_ALPHA:
        break;
    case BLEND_INV_DEST_ALPHA:
        break;
    case BLEND_DEST_COLOR:
        break;
    case BLEND_INV_DEST_COLOR:
        break;
    case BLEND_SRC_ALPHA_SAT:
        break;
    case BLEND_BLEND_FACTOR:
        break;
    case BLEND_INV_BLEND_FACTOR:
        break;
    case BLEND_SRC1_COLOR:
        break;
    case BLEND_INV_SRC1_COLOR:
        break;
    case BLEND_SRC1_ALPHA:
        break;
    case BLEND_INV_SRC1_ALPHA:
        break;
    default:
        break;
    }
}

inline GLenum getD3DBlendOp(BlendOp blendop)
{
    
    switch (blendop)
    {
    case engine::BLEND_OP_ADD:
        break;
    case engine::BLEND_OP_SUBTRACT:
        break;
    case engine::BLEND_OP_REV_SUBTRACT:
        break;
    case engine::BLEND_OP_MIN:
        break;
    case engine::BLEND_OP_MAX:
        break;
    default:
        break;
    }
}

GLStateManager::GLStateManager()
{
}

GLStateManager::~GLStateManager()
{
    shutdown();
}

void GLStateManager::init()
{
}

void GLStateManager::shutdown()
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


    //for (auto it : m_rasterizerStates)
    //{
    //    destroyRasterizerState(it.second);
    //}
    g_rasterizerStates.clear();

    m_rasterizerStates.clear();
}

IRasterizerState* GLStateManager::createRasterizerState(const RasterizerStateDesc& rasterizerDesc)
{
    // Find already created IRasterizerState
    // costyl
#if 0
	auto it = m_rasterizerStates.find(rasterizerDesc);
    if (it != m_rasterizerStates.end())
    {
        return (*it).second;
    }
#endif


    // create d3d object
    IRasterizerState* rasterizerState = allocateRasterizerState(rasterizerDesc);
    m_rasterizerStates.emplace(rasterizerDesc, rasterizerState);

	return rasterizerState;
}

void GLStateManager::destroyRasterizerState(IRasterizerState* rasterizerState)
{
    // nothing to destroy in OpenGL backend
}

void GLStateManager::setRasterizerState(IRasterizerState* rasterizerState)
{
    static IRasterizerState* previousRasterizerState = nullptr;
    if (previousRasterizerState != rasterizerState) {
        const RasterizerStateDesc& desc = rasterizerState->m_desc;
        glPolygonMode(getGLCullMode(desc.m_cullMode), getGLFillMode(desc.m_fillMode)); // #TODO: INSPECT PLEASE
        glCullFace(getGLCullMode(desc.m_cullMode));
        glFrontFace(desc.m_frontCCW ? GL_CCW : GL_CW); // #TODO: INSPECT PLEASE

      //if (desc.m_scissorEnable)
      //    glEnable(GL_SCISSOR_TEST);
      //else
            glDisable(GL_SCISSOR_TEST);

        previousRasterizerState = rasterizerState;
    }
}

ISamplerState* GLStateManager::createSamplerState(const SamplerDesc& samplerDesc)
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

void GLStateManager::destroySamplerState(ISamplerState* samplerState)
{
    // ???
    mem_delete(samplerState);
}

IBlendState* GLStateManager::createBlendState(const BlendStateDesc& blendStateDesc)
{
    // Find already created IBlendState
    auto it = m_blendStates.find(blendStateDesc);
    if (it != m_blendStates.end())
    {
        return (*it).second;
    }

    // create d3d object
    IBlendState* blendState = allocateBlendState(blendStateDesc);
    m_blendStates.emplace(blendStateDesc, blendState);
    return blendState;
}

void GLStateManager::destroyBlendState(IBlendState* blendState)
{
    // nothing to destroy in OpenGL backend
}

void GLStateManager::setBlendState(IBlendState* blendState, const float blendFactor[4], uint32_t sampleMask)
{
    //ID3D11BlendState* d3dBlendState = (ID3D11BlendState*)blendState;
    //g_d3d11Device->getDeviceContext()->OMSetBlendState(d3dBlendState, blendFactor, sampleMask);
}

IDepthStencilState* GLStateManager::createDepthStencilState(const DepthStencilDesc& desc)
{
    // Find already created IDepthStencilState
    auto it = m_depthStencilStates.find(desc);
    if (it != m_depthStencilStates.end())
    {
        return (*it).second;
    }


    // create d3d object
    IDepthStencilState* depthStencilState = allocateDepthStencilState(desc);
    m_depthStencilStates.emplace(desc, depthStencilState);
    return depthStencilState;
}

void GLStateManager::destroyDepthStencilState(IDepthStencilState* state)
{
    // nothing to destroy in OpenGL backend
}

void GLStateManager::setDepthStencilState(IDepthStencilState* state, uint32_t stencilRef)
{
    //ID3D11DepthStencilState* d3dDSState = (ID3D11DepthStencilState*)state;
    //g_d3d11Device->getDeviceContext()->OMSetDepthStencilState(d3dDSState, stencilRef);
}


}