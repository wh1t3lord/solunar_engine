#ifndef GRAPHICS_D3D11_D3D11RENDERER_H
#define GRAPHICS_D3D11_D3D11RENDERER_H

#include "graphics/renderer.h"
#include "graphics/core/statemanager.h"
// #include "graphics/shaderconstantmanager.h"
#include "d3d11drv/d3d11device.h"

namespace engine
{

struct D3D11RendererSingletonStorer
{
	// ShaderConstantManager m_constantManager;
};

class D3D11Renderer : public Renderer
{
public:
	D3D11Renderer();
	~D3D11Renderer();

	void init(void* window) override;
	void shutdown() override;

	void endFrame() override;

	void bindMaterialForMesh(MeshComponent* mesh, Material* material, MaterialInstance* materialInstance) override;

	void renderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh) override;
	void renderShadows(View* view) override;

	void takeScreenshot() override;

private:
	void createSwapChain(void* window);
	void createRasterizerState();

	void takeScreenshotInternal();

	void clearScreen() override;

	void renderStaticMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh);

private:
	D3D11RendererSingletonStorer m_d3d11SingletonStorer;

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