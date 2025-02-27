#ifndef D3D11STATEMANAGER_H
#define D3D11STATEMANAGER_H

#include "core/math/math_types.h"
#include "graphics/core/statemanager.h"
#include "graphics/core/texture.h"

namespace solunar
{

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

	//const bool operator<(const RasterizerStateKey& key) const;
};

//inline const bool RasterizerStateKey::operator<(const RasterizerStateKey& key) const
//{
//	return m_rasterizerDesc.m_fillMode > key.m_rasterizerDesc.m_fillMode &&
//		m_rasterizerDesc.m_cullMode > key.m_rasterizerDesc.m_cullMode &&
//		m_rasterizerDesc.m_frontCCW > key.m_rasterizerDesc.m_frontCCW &&
//		m_rasterizerDesc.m_depthBias > key.m_rasterizerDesc.m_depthBias &&
//		m_rasterizerDesc.m_depthBiasClamp > key.m_rasterizerDesc.m_depthBiasClamp &&
//		m_rasterizerDesc.m_slopeScaledDepthBias > key.m_rasterizerDesc.m_slopeScaledDepthBias &&
//		m_rasterizerDesc.m_depthClipEnable > key.m_rasterizerDesc.m_depthClipEnable &&
//		m_rasterizerDesc.m_scissorEnable > key.m_rasterizerDesc.m_scissorEnable &&
//		m_rasterizerDesc.m_multisampleEnable > key.m_rasterizerDesc.m_multisampleEnable &&
//		m_rasterizerDesc.m_antialiasedLineEnable > key.m_rasterizerDesc.m_antialiasedLineEnable;
//}

//inline const bool RasterizerStateKey::operator<(const RasterizerStateKey& key) const
//{
//	return m_rasterizerDesc.m_fillMode < key.m_rasterizerDesc.m_fillMode &&
//		m_rasterizerDesc.m_cullMode < key.m_rasterizerDesc.m_cullMode &&
//		m_rasterizerDesc.m_frontCCW < key.m_rasterizerDesc.m_frontCCW &&
//		m_rasterizerDesc.m_depthBias < key.m_rasterizerDesc.m_depthBias &&
//		m_rasterizerDesc.m_depthBiasClamp <= key.m_rasterizerDesc.m_depthBiasClamp &&
//		m_rasterizerDesc.m_slopeScaledDepthBias <= key.m_rasterizerDesc.m_slopeScaledDepthBias &&
//		m_rasterizerDesc.m_depthClipEnable < key.m_rasterizerDesc.m_depthClipEnable &&
//		m_rasterizerDesc.m_scissorEnable < key.m_rasterizerDesc.m_scissorEnable &&
//		m_rasterizerDesc.m_multisampleEnable < key.m_rasterizerDesc.m_multisampleEnable &&
//		m_rasterizerDesc.m_antialiasedLineEnable < key.m_rasterizerDesc.m_antialiasedLineEnable;
//}

//inline const bool RasterizerStateKey::operator<(const RasterizerStateKey& key) const
//{
//	return	m_rasterizerDesc.m_fillMode == key.m_rasterizerDesc.m_fillMode &&
//			m_rasterizerDesc.m_cullMode == key.m_rasterizerDesc.m_cullMode &&
//			m_rasterizerDesc.m_frontCCW == key.m_rasterizerDesc.m_frontCCW &&
//			m_rasterizerDesc.m_depthBias == key.m_rasterizerDesc.m_depthBias &&
//			m_rasterizerDesc.m_depthBiasClamp == key.m_rasterizerDesc.m_depthBiasClamp &&
//			m_rasterizerDesc.m_slopeScaledDepthBias == key.m_rasterizerDesc.m_slopeScaledDepthBias &&
//			m_rasterizerDesc.m_depthClipEnable == key.m_rasterizerDesc.m_depthClipEnable &&
//			m_rasterizerDesc.m_scissorEnable == key.m_rasterizerDesc.m_scissorEnable &&
//			m_rasterizerDesc.m_multisampleEnable == key.m_rasterizerDesc.m_multisampleEnable &&
//			m_rasterizerDesc.m_antialiasedLineEnable == key.m_rasterizerDesc.m_antialiasedLineEnable;
//}

//inline bool operator==(const RasterizerStateKey& a, const RasterizerStateKey& b)
//{
//	return	a.m_rasterizerDesc.m_fillMode == b.m_rasterizerDesc.m_fillMode &&
//			a.m_rasterizerDesc.m_cullMode == b.m_rasterizerDesc.m_cullMode &&
//			a.m_rasterizerDesc.m_frontCCW == b.m_rasterizerDesc.m_frontCCW &&
//			a.m_rasterizerDesc.m_depthBias == b.m_rasterizerDesc.m_depthBias &&
//			a.m_rasterizerDesc.m_depthBiasClamp == b.m_rasterizerDesc.m_depthBiasClamp &&
//			a.m_rasterizerDesc.m_slopeScaledDepthBias == b.m_rasterizerDesc.m_slopeScaledDepthBias &&
//			a.m_rasterizerDesc.m_depthClipEnable == b.m_rasterizerDesc.m_depthClipEnable &&
//			a.m_rasterizerDesc.m_scissorEnable == b.m_rasterizerDesc.m_scissorEnable &&
//			a.m_rasterizerDesc.m_multisampleEnable == b.m_rasterizerDesc.m_multisampleEnable &&
//			a.m_rasterizerDesc.m_antialiasedLineEnable == b.m_rasterizerDesc.m_antialiasedLineEnable;
//}
//
//inline bool operator!=(const RasterizerStateKey& a, const RasterizerStateKey& b)
//{
//	return	a.m_rasterizerDesc.m_fillMode != b.m_rasterizerDesc.m_fillMode &&
//		a.m_rasterizerDesc.m_cullMode != b.m_rasterizerDesc.m_cullMode &&
//		a.m_rasterizerDesc.m_frontCCW != b.m_rasterizerDesc.m_frontCCW &&
//		a.m_rasterizerDesc.m_depthBias != b.m_rasterizerDesc.m_depthBias &&
//		a.m_rasterizerDesc.m_depthBiasClamp != b.m_rasterizerDesc.m_depthBiasClamp &&
//		a.m_rasterizerDesc.m_slopeScaledDepthBias != b.m_rasterizerDesc.m_slopeScaledDepthBias &&
//		a.m_rasterizerDesc.m_depthClipEnable != b.m_rasterizerDesc.m_depthClipEnable &&
//		a.m_rasterizerDesc.m_scissorEnable != b.m_rasterizerDesc.m_scissorEnable &&
//		a.m_rasterizerDesc.m_multisampleEnable != b.m_rasterizerDesc.m_multisampleEnable &&
//		a.m_rasterizerDesc.m_antialiasedLineEnable != b.m_rasterizerDesc.m_antialiasedLineEnable;
//}

struct SamplerStateKey
{
	SamplerDesc m_samplerDesc;

	SamplerStateKey() = default;
	SamplerStateKey(const SamplerDesc& samplerDesc) : m_samplerDesc(samplerDesc) {}

	//const bool operator<(const SamplerStateKey& key) const;
};

//inline const bool SamplerStateKey::operator<(const SamplerStateKey& key) const
//{
//	return	m_samplerDesc.m_minFilter < key.m_samplerDesc.m_minFilter &&
//			m_samplerDesc.m_magFilter < key.m_samplerDesc.m_magFilter &&
//			m_samplerDesc.m_wrapS < key.m_samplerDesc.m_wrapS &&
//			m_samplerDesc.m_wrapT < key.m_samplerDesc.m_wrapT &&
//			m_samplerDesc.m_wrapRepeat < key.m_samplerDesc.m_wrapRepeat &&
//			m_samplerDesc.m_anisotropyLevel <= key.m_samplerDesc.m_anisotropyLevel;			
//}

struct BlendStateKey
{
	BlendStateDesc m_blendStateDesc;

	BlendStateKey() = default;
	BlendStateKey(const BlendStateDesc& blendDesc) : m_blendStateDesc(blendDesc) {}

	bool compareRenderTargetBlendDesc(const RenderTargetBlendDesc& desc1, const RenderTargetBlendDesc& desc2) const;

	//const bool operator<(const BlendStateKey& key) const;
};

inline bool BlendStateKey::compareRenderTargetBlendDesc(const RenderTargetBlendDesc& desc1, 
														const RenderTargetBlendDesc& desc2) const
{
	return	desc1.m_blendEnable < desc2.m_blendEnable &&
			desc1.m_srcBlend < desc2.m_srcBlend &&
			desc1.m_destBlend < desc2.m_destBlend &&
			desc1.m_blendOp < desc2.m_blendOp &&
			desc1.m_srcBlendAlpha < desc2.m_srcBlendAlpha &&
			desc1.m_destBlendAlpha < desc2.m_destBlendAlpha &&
			desc1.m_blendOpAlpha < desc2.m_blendOpAlpha &&
			desc1.m_renderTargetWriteMask < desc2.m_renderTargetWriteMask;
}

//inline const bool BlendStateKey::operator<(const BlendStateKey& key) const
//{
//	return	m_blendStateDesc.m_alphaToCoverageEnable < key.m_blendStateDesc.m_alphaToCoverageEnable &&
//			m_blendStateDesc.m_independentBlendEnable < key.m_blendStateDesc.m_independentBlendEnable &&
//			compareRenderTargetBlendDesc(m_blendStateDesc.m_renderTarget[0], key.m_blendStateDesc.m_renderTarget[0]) &&
//			compareRenderTargetBlendDesc(m_blendStateDesc.m_renderTarget[1], key.m_blendStateDesc.m_renderTarget[1]) &&
//			compareRenderTargetBlendDesc(m_blendStateDesc.m_renderTarget[2], key.m_blendStateDesc.m_renderTarget[2]) &&
//			compareRenderTargetBlendDesc(m_blendStateDesc.m_renderTarget[3], key.m_blendStateDesc.m_renderTarget[3]) &&
//			compareRenderTargetBlendDesc(m_blendStateDesc.m_renderTarget[4], key.m_blendStateDesc.m_renderTarget[4]) &&
//			compareRenderTargetBlendDesc(m_blendStateDesc.m_renderTarget[5], key.m_blendStateDesc.m_renderTarget[5]) &&
//			compareRenderTargetBlendDesc(m_blendStateDesc.m_renderTarget[6], key.m_blendStateDesc.m_renderTarget[6]) &&
//			compareRenderTargetBlendDesc(m_blendStateDesc.m_renderTarget[7], key.m_blendStateDesc.m_renderTarget[7]);
//}

struct DepthStencilKey
{
	DepthStencilDesc m_depthStencilDesc;

	DepthStencilKey() = default;
	DepthStencilKey(const DepthStencilDesc& depthStencilDesc) : m_depthStencilDesc(depthStencilDesc) {}

	//const bool operator<(const DepthStencilKey& key) const;
};

//inline const bool DepthStencilKey::operator<(const DepthStencilKey& key) const
//{
//	return	m_depthStencilDesc.m_depthEnable < key.m_depthStencilDesc.m_depthEnable &&
//			m_depthStencilDesc.m_depthWriteMask < key.m_depthStencilDesc.m_depthWriteMask &&
//			m_depthStencilDesc.m_depthFunc < key.m_depthStencilDesc.m_depthFunc &&
//			m_depthStencilDesc.m_stencilEnable < key.m_depthStencilDesc.m_stencilEnable &&
//			m_depthStencilDesc.m_stencilReadMask < key.m_depthStencilDesc.m_stencilReadMask &&
//			m_depthStencilDesc.m_stencilWriteMask < key.m_depthStencilDesc.m_stencilWriteMask &&
//			m_depthStencilDesc.m_frontFace.m_stencilFailOp < key.m_depthStencilDesc.m_frontFace.m_stencilFailOp &&
//			m_depthStencilDesc.m_frontFace.m_stencilDepthFailOp < key.m_depthStencilDesc.m_frontFace.m_stencilDepthFailOp &&
//			m_depthStencilDesc.m_frontFace.m_stencilPassOp < key.m_depthStencilDesc.m_frontFace.m_stencilPassOp &&
//			m_depthStencilDesc.m_frontFace.m_stencilFunc < key.m_depthStencilDesc.m_frontFace.m_stencilFunc &&
//			m_depthStencilDesc.m_backFace.m_stencilFailOp < key.m_depthStencilDesc.m_backFace.m_stencilFailOp &&
//			m_depthStencilDesc.m_backFace.m_stencilDepthFailOp < key.m_depthStencilDesc.m_backFace.m_stencilDepthFailOp &&
//			m_depthStencilDesc.m_backFace.m_stencilPassOp < key.m_depthStencilDesc.m_backFace.m_stencilPassOp &&
//			m_depthStencilDesc.m_backFace.m_stencilFunc < key.m_depthStencilDesc.m_backFace.m_stencilFunc;
//}

class D3D11StateManager : public IStateManager
{
public:
	D3D11StateManager();
	~D3D11StateManager();

	void Init();
	void Shutdown();

	IRasterizerState* CreateRasterizerState(const RasterizerStateDesc& rasterizerDesc) override;
	void DestroyRasterizerState(IRasterizerState* rasterizerState) override;
	void SetRasterizerState(IRasterizerState* rasterizerState) override;

	ISamplerState* CreateSamplerState(const SamplerDesc& samplerDesc) override;
	void DestroySamplerState(ISamplerState* samplerState) override;

	IBlendState* CreateBlendState(const BlendStateDesc& blendStateDesc) override;
	void DestroyBlendState(IBlendState* blendState) override;
	void SetBlendState(IBlendState* blendState, const float blendFactor[4], uint32_t sampleMask) override;

	IDepthStencilState* CreateDepthStencilState(const DepthStencilDesc& desc) override;
	void DestroyDepthStencilState(IDepthStencilState* state) override;
	void SetDepthStencilState(IDepthStencilState* state, uint32_t stencilRef) override;

private:
	std::map<RasterizerStateKey, IRasterizerState*, AnyStructComparer<RasterizerStateKey>> m_rasterizerStates;
	std::map<SamplerStateKey, ISamplerState*, AnyStructComparer<SamplerStateKey>> m_samplerStates;
	std::map<BlendStateKey, IBlendState*, AnyStructComparer<BlendStateKey>> m_blendStates;
	std::map<DepthStencilKey, IDepthStencilState*, AnyStructComparer<DepthStencilKey>> m_depthStencilStates;
	
};

#define g_d3d11StateManager ((D3D11StateManager*)g_stateManager)

}

#endif // !D3D11STATEMANAGER_H
