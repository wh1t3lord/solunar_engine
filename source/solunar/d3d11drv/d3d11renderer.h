#ifndef GRAPHICS_D3D11_D3D11RENDERER_H
#define GRAPHICS_D3D11_D3D11RENDERER_H

#include "graphics/renderer.h"
#include "graphics/core/statemanager.h"
// #include "graphics/shaderconstantmanager.h"
#include "d3d11drv/d3d11device.h"

namespace engine
{

struct RendererSingletonStorer
{
	// ShaderConstantManager m_constantManager;
};

class D3D11Renderer : public Renderer
{
public:
	D3D11Renderer();
	~D3D11Renderer();

	void init() override;
	void shutdown() override;

	void endFrame() override;

	void bindMaterialForMesh(MeshComponent* mesh, Material* material, MaterialInstance* materialInstance) override;

	void renderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh) override;
	void renderShadows(View* view) override;

	void takeScreenshot() override;

private:
	void createSwapChain();
	void createRasterizerState();

	void takeScreenshotInternal();

	void clearScreen() override;
	void clearRenderTarget(IRenderTarget* renderTarget) override;
	void setSwapChainRenderTarget() override;

	void renderStaticMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh);

private:
	RendererSingletonStorer m_singletonStorer;

	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilTexture;
	ID3D11DepthStencilView* m_depthStencilView;
	
	ID3D11DepthStencilState* m_depthStencilState;

	IRasterizerState* m_rasterizerState;

	IDXGISwapChain* m_swapChain;

	bool m_takeScreenshot;

};

}

#endif