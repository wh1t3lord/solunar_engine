#ifndef GRAPHICS_D3D11_D3D11RENDERER_H
#define GRAPHICS_D3D11_D3D11RENDERER_H

#include "graphics/renderer.h"
#include "graphics/core/statemanager.h"
// #include "graphics/shaderconstantmanager.h"
#include "d3d11drv/d3d11device.h"

namespace solunar
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

	void Init() override;
	void Shutdown() override;

	void EndFrame() override;

	void BindMaterialForMesh(MeshComponent* mesh, Material* material, IMaterialInstance* materialInstance) override;

	void RenderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh) override;
	void renderShadows(View* view) override;

	void TakeScreenshot() override;

private:
	void createSwapChain();

	void takeScreenshotInternal();

	void clearScreen() override;
	void ClearRenderTarget(IRenderTarget* renderTarget) override;
	void setSwapChainRenderTarget() override;

	void RenderStaticMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh);
	void RenderAnimatedMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh);

private:
	RendererSingletonStorer m_singletonStorer;

	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilTexture;
	ID3D11DepthStencilView* m_depthStencilView;
	
	ID3D11DepthStencilState* m_depthStencilState;

	IDXGISwapChain* m_swapChain;

	bool m_takeScreenshot;

};

}

#endif