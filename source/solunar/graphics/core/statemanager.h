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

	virtual IRasterizerState* CreateRasterizerState(const RasterizerStateDesc& rasterizerDesc) = 0;
	virtual void DestroyRasterizerState(IRasterizerState* rasterizerState) = 0;
	virtual void SetRasterizerState(IRasterizerState* rasterizerState) = 0;

	virtual ISamplerState* CreateSamplerState(const SamplerDesc& samplerDesc) = 0;
	virtual void DestroySamplerState(ISamplerState* samplerState) = 0;

	virtual IBlendState* CreateBlendState(const BlendStateDesc& blendStateDesc) = 0;
	virtual void DestroyBlendState(IBlendState* blendState) = 0;
	virtual void SetBlendState(IBlendState* blendState, const float blendFactor[4], uint32_t sampleMask) = 0;

	virtual IDepthStencilState* CreateDepthStencilState(const DepthStencilDesc& desc) = 0;
	virtual void DestroyDepthStencilState(IDepthStencilState* state) = 0;
	virtual void SetDepthStencilState(IDepthStencilState* state, uint32_t stencilRef) = 0;
};

extern IStateManager* g_stateManager;

}

#endif