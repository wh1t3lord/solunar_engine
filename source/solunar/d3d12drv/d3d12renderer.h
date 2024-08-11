#ifndef D3D12DRV_D3D12RENDERER_H
#define D3D12DRV_D3D12RENDERER_H

#include "graphics/renderer.h"

namespace solunar
{

class D3D12Renderer : public Renderer
{
public:
	D3D12Renderer();
	~D3D12Renderer();

	void Init() override;
	void Shutdown() override;

	void EndFrame() override;

	void bindMaterialForMesh(MeshComponent* mesh, Material* material, IMaterialInstance* materialInstance) override;

	void renderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh) override;
	void renderShadows(View* view) override;

	void TakeScreenshot() override;

private:
	void createSwapChain();
	void createRasterizerState();

	void takeScreenshotInternal();

	void clearScreen() override;
	void clearRenderTarget(IRenderTarget* renderTarget) override;
	void setSwapChainRenderTarget() override;

	void renderStaticMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh);
	void renderAnimatedMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh);

private:
	//IRasterizerState* m_rasterizerState;
	IDXGISwapChain* m_swapChain;

	bool m_takeScreenshot;
};

}

#endif