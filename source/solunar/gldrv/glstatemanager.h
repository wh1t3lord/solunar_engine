#ifndef GLSTATEMANAGER_H
#define GLSTATEMANAGER_H

#include "core/math/math_types.h"
#include "graphics/core/statemanager.h"
#include "graphics/core/texture.h"

namespace engine
{

// proximity objects for state manager

// implementation of IRasterizerState interface
struct IRasterizerState
{
    RasterizerStateDesc m_desc;
};

// implementation of IBlendState interface
struct IBlendState
{
    BlendStateDesc m_desc;
};

// implementation of IDepthStencilState interface
struct IDepthStencilState
{
    DepthStencilDesc m_desc;
};

template <typename T>
struct AnyStructComparer
{
    bool operator()( const T & Left, const T & Right ) const
    {
        // comparison logic goes here
        return memcmp( &Left, &Right, sizeof( Right ) ) < 0;
    }
};

struct RasterizerStateKey
{
	RasterizerStateDesc m_rasterizerDesc;

	RasterizerStateKey() = default;
	RasterizerStateKey(const RasterizerStateDesc& desc) : m_rasterizerDesc(desc) {}
};

struct SamplerStateKey
{
	SamplerDesc m_samplerDesc;

	SamplerStateKey() = default;
	SamplerStateKey(const SamplerDesc& samplerDesc) : m_samplerDesc(samplerDesc) {}
};

struct BlendStateKey
{
	BlendStateDesc m_blendStateDesc;

	BlendStateKey() = default;
	BlendStateKey(const BlendStateDesc& blendDesc) : m_blendStateDesc(blendDesc) {}
};

struct DepthStencilKey
{
	DepthStencilDesc m_depthStencilDesc;

	DepthStencilKey() = default;
	DepthStencilKey(const DepthStencilDesc& depthStencilDesc) : m_depthStencilDesc(depthStencilDesc) {}
};

class GLStateManager : public IStateManager
{
public:
	GLStateManager();
	~GLStateManager();

	void init();
	void shutdown();

	IRasterizerState* createRasterizerState(const RasterizerStateDesc& rasterizerDesc) override;
	void destroyRasterizerState(IRasterizerState* rasterizerState) override;
	void setRasterizerState(IRasterizerState* rasterizerState) override;

	ISamplerState* createSamplerState(const SamplerDesc& samplerDesc) override;
	void destroySamplerState(ISamplerState* samplerState) override;

	IBlendState* createBlendState(const BlendStateDesc& blendStateDesc) override;
	void destroyBlendState(IBlendState* blendState) override;
	void setBlendState(IBlendState* blendState, const float blendFactor[4], uint32_t sampleMask) override;

	IDepthStencilState* createDepthStencilState(const DepthStencilDesc& desc) override;
	void destroyDepthStencilState(IDepthStencilState* state) override;
	void setDepthStencilState(IDepthStencilState* state, uint32_t stencilRef) override;

private:
	//std::map<RasterizerStateKey, IRasterizerState, AnyStructComparer<RasterizerStateKey>> m_rasterizerStates;
	//std::map<SamplerStateKey, ISamplerState, AnyStructComparer<SamplerStateKey>> m_samplerStates;
	//std::map<BlendStateKey, IBlendState, AnyStructComparer<BlendStateKey>> m_blendStates;
	//std::map<DepthStencilKey, IDepthStencilState, AnyStructComparer<DepthStencilKey>> m_depthStencilStates;

	std::vector<IRasterizerState> m_rasterizerStates;
	std::map<SamplerStateKey, ISamplerState*, AnyStructComparer<SamplerStateKey>> m_samplerStates;
	std::vector<IBlendState> m_blendStates;
	std::vector<IDepthStencilState> m_depthStencilStates;
	
};

#define g_glStateManager ((GLStateManager*)g_stateManager)

}

#endif // !D3D11STATEMANAGER_H
