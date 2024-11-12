#ifndef RENDERER_H
#define RENDERER_H

namespace solunar
{
	class View;
	class GraphicsDevice;
	class PostProcessingRendererOld;
	class SkyMeshComponent;
	class StaticMeshComponent;
	class ITexture2D;
	class IRenderTarget;
	class IShaderProgram;
	class World;
	class MeshComponent;
	class Material;
	class IMaterialInstance;
	class GraphicsWorld;

	enum class RendererViewMode
	{
		Wireframe,
		Unlit,
		Lit
	};

	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();
		void initFramebuffer(View* view);
		void initInplaceResources();

		virtual void Init();
		virtual void Shutdown();

	//	void initForView(View* view);
	
		void SetView(View* view) { m_view = view; }
		View* GetView() { return m_view; }

		virtual void BeginFrame();
		virtual void EndFrame();

		void RenderWorld(View* view);
		void RenderShadows(View* view);

		void RenderView(View* view);
		void RenderSky(View* view, SkyMeshComponent* skyMesh);
		void SetupLights(GraphicsWorld* graphicsWorld);
	
		virtual void BindMaterialForMesh(MeshComponent* mesh, Material* material, IMaterialInstance* materialInstance) = 0;

		virtual void RenderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh) = 0;
		virtual void renderShadows(View* view) = 0;

		virtual void TakeScreenshot() = 0;
		
		virtual void clearScreen() = 0;
		virtual void ClearRenderTarget( IRenderTarget* renderTarget ) = 0;
	 
		virtual void setSwapChainRenderTarget() = 0;

		// Too simple but i dont regret
		void SetRenderMode(RendererViewMode mode) { m_currentViewMode = mode; }
		RendererViewMode GetRenderMode() { return m_currentViewMode; }

		void ToggleShowingWireframe();
		void ToggleShowOctree();

		IRenderTarget* getSwapChainRenderTarget() { return m_swapChainRenderTarget; }

		void RenderLoadscreen();

		// New API

		void SetDefaultRenderState();

	protected:
		ITexture2D* m_screenColorTexture;
		ITexture2D* m_screenDepthTexture;
		IRenderTarget* m_screenRenderTarget;
		
		IRenderTarget* m_swapChainRenderTarget;

		View* m_view;

		RendererViewMode m_currentViewMode;

		bool m_meshPolysWireframe;
		bool m_showOctree;
	};

	extern Renderer* g_renderer;

	void createRenderer();
	void destroyRenderer();

	std::weak_ptr<Material> GetDefaultMaterial();

	class ISamplerState;
	extern ISamplerState* g_defaultSampler;

}

#endif // !RENDERER_H
