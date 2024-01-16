#ifndef D3D11STATEMANAGER_H
#define D3D11STATEMANAGER_H

#include "core/math/math_types.h"
#include "graphics/core/statemanager.h"

namespace engine
{

inline bool FAreSame(double a, double b)
{
	return fabs(a - b) < maths::EPSILON;
}

struct RasterizerStateKey
{
	RasterizerStateDesc m_rasterizerDesc;

	const bool operator<(const RasterizerStateKey& key) const;
	bool operator()(const RasterizerStateKey& lhs, const RasterizerStateKey& rhs) const;
};

bool operator<(const RasterizerStateKey& lhs, const RasterizerStateKey& rhs)
{
	return	lhs.m_rasterizerDesc.m_fillMode < rhs.m_rasterizerDesc.m_fillMode &&
			lhs.m_rasterizerDesc.m_cullMode < rhs.m_rasterizerDesc.m_cullMode &&
			lhs.m_rasterizerDesc.m_frontCCW < rhs.m_rasterizerDesc.m_frontCCW &&
			lhs.m_rasterizerDesc.m_depthBias < rhs.m_rasterizerDesc.m_depthBias &&
			lhs.m_rasterizerDesc.m_depthBiasClamp <= rhs.m_rasterizerDesc.m_depthBiasClamp &&
			lhs.m_rasterizerDesc.m_slopeScaledDepthBias <= rhs.m_rasterizerDesc.m_slopeScaledDepthBias &&
			lhs.m_rasterizerDesc.m_depthClipEnable < rhs.m_rasterizerDesc.m_depthClipEnable &&
			lhs.m_rasterizerDesc.m_scissorEnable < rhs.m_rasterizerDesc.m_scissorEnable &&
			lhs.m_rasterizerDesc.m_multisampleEnable < rhs.m_rasterizerDesc.m_multisampleEnable &&
			lhs.m_rasterizerDesc.m_antialiasedLineEnable < rhs.m_rasterizerDesc.m_antialiasedLineEnable;
}

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

inline bool RasterizerStateKey::operator()(const RasterizerStateKey& lhs, const RasterizerStateKey& rhs) const
{
	return	lhs.m_rasterizerDesc.m_fillMode < rhs.m_rasterizerDesc.m_fillMode &&
			lhs.m_rasterizerDesc.m_cullMode < rhs.m_rasterizerDesc.m_cullMode &&
			lhs.m_rasterizerDesc.m_frontCCW < rhs.m_rasterizerDesc.m_frontCCW &&
			lhs.m_rasterizerDesc.m_depthBias < rhs.m_rasterizerDesc.m_depthBias &&
			lhs.m_rasterizerDesc.m_depthBiasClamp <= rhs.m_rasterizerDesc.m_depthBiasClamp &&
			lhs.m_rasterizerDesc.m_slopeScaledDepthBias <= rhs.m_rasterizerDesc.m_slopeScaledDepthBias &&
			lhs.m_rasterizerDesc.m_depthClipEnable < rhs.m_rasterizerDesc.m_depthClipEnable &&
			lhs.m_rasterizerDesc.m_scissorEnable < rhs.m_rasterizerDesc.m_scissorEnable &&
			lhs.m_rasterizerDesc.m_multisampleEnable < rhs.m_rasterizerDesc.m_multisampleEnable &&
			lhs.m_rasterizerDesc.m_antialiasedLineEnable < rhs.m_rasterizerDesc.m_antialiasedLineEnable;
}

class D3D11StateManager : public IStateManager
{
public:
	D3D11StateManager();
	~D3D11StateManager();

	IRasterizerState* createRasterizerState(const RasterizerStateDesc& rasterizerDesc) override;

private:
	std::map<RasterizerStateKey, IRasterizerState*> m_rasterizerStates;
};

}

#endif // !D3D11STATEMANAGER_H
