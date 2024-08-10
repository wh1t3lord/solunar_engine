#ifndef CORE_STATEMANAGER_H
#define CORE_STATEMANAGER_H

namespace solunar
{

//class IRasterizerState
//{
//public:
//	virtual ~IRasterizerState() = default;
//};
//
//class IDepthStencilState
//{
//public:
//	virtual ~IDepthStencilState() = default;
//};
//
//class IBlendState
//{
//public:
//	virtual ~IBlendState() = default;
//};

struct IRasterizerState;
struct IBlendState;
struct IDepthStencilState;
class ISamplerState;

class IStateManager
{
public:
	virtual ~IStateManager() = default;

	virtual IRasterizerState* createRasterizerState(const RasterizerStateDesc& rasterizerDesc) = 0;
	virtual void destroyRasterizerState(IRasterizerState* rasterizerState) = 0;
	virtual void setRasterizerState(IRasterizerState* rasterizerState) = 0;

	virtual ISamplerState* createSamplerState(const SamplerDesc& samplerDesc) = 0;
	virtual void destroySamplerState(ISamplerState* samplerState) = 0;

	virtual IBlendState* createBlendState(const BlendStateDesc& blendStateDesc) = 0;
	virtual void destroyBlendState(IBlendState* blendState) = 0;
	virtual void setBlendState(IBlendState* blendState, const float blendFactor[4], uint32_t sampleMask) = 0;

	virtual IDepthStencilState* createDepthStencilState(const DepthStencilDesc& desc) = 0;
	virtual void destroyDepthStencilState(IDepthStencilState* state) = 0;
	virtual void setDepthStencilState(IDepthStencilState* state, uint32_t stencilRef) = 0;
};

extern IStateManager* g_stateManager;

}

#endif