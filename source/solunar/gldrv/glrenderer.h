#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "graphics/renderer.h"

#include "graphics/shaderconstantmanager.h"

namespace engine {

struct GraphicsSingletonStorer
{
//	ShaderConstantManager m_constantManager;
};

class GLRenderer : public Renderer
{
public:
	GLRenderer();
	~GLRenderer();

	void init() override;
	void shutdown() override;

	void endFrame() override;
	
	void takeScreenshot() override;

	void bindMaterialForMesh(MeshComponent* mesh, Material* material, IMaterialInstance* materialInstance);

	void renderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh) override;
	void renderShadows(View* view) override;

	void updateGlobalConstants(World* world, IShaderProgram* shader);

private:
	void renderStaticMesh(View* view, MeshComponent* mesh); 
	void renderStaticMeshCustomShader(View* view, MeshComponent* mesh, IShaderProgram* customShader); 

	void updatePointLightsConstants(World* world, IShaderProgram* shader);

	void takeScreenshotInternal();


private:
	GraphicsSingletonStorer m_singletonStorer;

	ITexture2D* m_screenColorTexture;
	ITexture2D* m_screenDepthTexture;
	IRenderTarget* m_screenRenderTarget;

	std::string m_screenshotFilename;
	bool m_takeScreenshot;


	// Inherited via Renderer
	void clearScreen() override;

	void clearRenderTarget(IRenderTarget* renderTarget) override;

	void setSwapChainRenderTarget() override;

};
	
}

#endif