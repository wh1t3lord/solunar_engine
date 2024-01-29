#ifndef D3D11STATEMANAGER_H
#define D3D11STATEMANAGER_H

#include "core/math/math_types.h"
#include "graphics/core/statemanager.h"
#include "graphics/core/texture.h"

namespace engine
{

struct RasterizerStateKey
{
	RasterizerStateDesc m_rasterizerDesc;

	RasterizerStateKey() = default;
	RasterizerStateKey(const RasterizerStateDesc& desc) : m_rasterizerDesc(desc) {}

	const bool operator<(const RasterizerStateKey& key) const;
};

inline const bool RasterizerStateKey::operator<(const RasterizerStateKey& key) const
{
	return	m_rasterizerDesc.m_fillMode < key.m_rasterizerDesc.m_fillMode &&
			m_rasterizerDesc.m_cullMode < key.m_rasterizerDesc.m_cullMode &&
			m_rasterizerDesc.m_frontCCW < key.m_rasterizerDesc.m_frontCCW &&
			m_rasterizerDesc.m_depthBias < key.m_rasterizerDesc.m_depthBias &&
			m_rasterizerDesc.m_depthBiasClamp <= key.m_rasterizerDesc.m_depthBiasClamp &&
			m_rasterizerDesc.m_slopeScaledDepthBias <= key.m_rasterizerDesc.m_slopeScaledDepthBias &&
			m_rasterizerDesc.m_depthClipEnable < key.m_rasterizerDesc.m_depthClipEnable &&
			m_rasterizerDesc.m_scissorEnable < key.m_rasterizerDesc.m_scissorEnable &&
			m_rasterizerDesc.m_multisampleEnable < key.m_rasterizerDesc.m_multisampleEnable &&
			m_rasterizerDesc.m_antialiasedLineEnable < key.m_rasterizerDesc.m_antialiasedLineEnable;
}

struct SamplerStateKey
{
	SamplerDesc m_samplerDesc;

	SamplerStateKey() = default;
	SamplerStateKey(const SamplerDesc& samplerDesc) : m_samplerDesc(samplerDesc) {}

	const bool operator<(const SamplerStateKey& key) const;
};

inline const bool SamplerStateKey::operator<(const SamplerStateKey& key) const
{
	return	m_samplerDesc.m_minFilter < key.m_samplerDesc.m_minFilter &&
			m_samplerDesc.m_magFilter < key.m_samplerDesc.m_magFilter &&
			m_samplerDesc.m_wrapS < key.m_samplerDesc.m_wrapS &&
			m_samplerDesc.m_wrapT < key.m_samplerDesc.m_wrapT &&
			m_samplerDesc.m_wrapRepeat < key.m_samplerDesc.m_wrapRepeat &&
			m_samplerDesc.m_anisotropyLevel <= key.m_samplerDesc.m_anisotropyLevel;			
}

class D3D11StateManager : public IStateManager
{
public:
	D3D11StateManager();
	~D3D11StateManager();

	void init();
	void shutdown();

	IRasterizerState* createRasterizerState(const RasterizerStateDesc& rasterizerDesc) override;
	void destroyRasterizerState(IRasterizerState* rasterizerState) override;
	void setRasterizerState(IRasterizerState* rasterizerState) override;

	ISamplerState* createSamplerState(const SamplerDesc& samplerDesc) override;
	void destroySamplerState(ISamplerState* samplerState) override;

private:
	std::map<RasterizerStateKey, IRasterizerState*> m_rasterizerStates;
	std::map<SamplerStateKey, ISamplerState*> m_samplerStates;
	
};

#define g_d3d11StateManager ((D3D11StateManager*)g_stateManager)

}

#endif // !D3D11STATEMANAGER_H
