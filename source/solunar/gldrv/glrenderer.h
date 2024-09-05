#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "graphics/renderer.h"

#include "graphics/shaderconstantmanager.h"

namespace solunar {

struct RendererSingletonStorer
{
	//ShaderConstantManager m_constantManager;
};

struct IRasterizerState;

class GLRenderer : public Renderer
{
public:
	GLRenderer();
	~GLRenderer();

	void Init() override;
	void Shutdown() override;

	void BeginFrame() override;
	void EndFrame() override;
	
	void TakeScreenshot() override;

	void BindMaterialForMesh(MeshComponent* mesh, Material* material, IMaterialInstance* materialInstance);

	void RenderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh) override;
	void renderShadows(View* view) override;

private:
	void RenderStaticMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh);
	void RenderAnimatedMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh);
	void renderStaticMeshCustomShader(View* view, MeshComponent* mesh, IShaderProgram* customShader); 

	void setRenderModeForShader(IShaderProgram* shaderProgram);

	void takeScreenshotInternal();


private:
	RendererSingletonStorer m_singletonStorer;

	ITexture2D* m_screenColorTexture;
	ITexture2D* m_screenDepthTexture;
	IRenderTarget* m_screenRenderTarget;
	IRasterizerState* m_rasterizerState;

	std::string m_screenshotFilename;
	bool m_makeScreenshot;


	// ������������ ����� Renderer
	virtual void clearScreen() override;


	// Inherited via Renderer
	void ClearRenderTarget(IRenderTarget* renderTarget) override;

	void setSwapChainRenderTarget() override;

};
	
}

#endif