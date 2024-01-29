#ifndef CORE_STATEMANAGER_H
#define CORE_STATEMANAGER_H

namespace engine
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
};

extern IStateManager* g_stateManager;

}

#endif