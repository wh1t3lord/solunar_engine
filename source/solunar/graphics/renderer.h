#ifndef RENDERER_H
#define RENDERER_H

namespace engine
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

	enum ConstantBufferBindings
	{
		ConstantBufferBindings_Scene,
		ConstantBufferBindings_Skinning,
		CBBindings_DirectionalLight,
		ConstantBufferBindings_PointLights,
		ConstantBufferBinding_LightData,
	};

	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();
		void initFramebuffer(View* view);
		void initInplaceResources();

		virtual void init();
		virtual void shutdown();

	//	void initForView(View* view);
	
		void setView(View* view) { m_view = view; }
		View* getView() { return m_view; }

		virtual void beginFrame();
		virtual void endFrame();

		void renderWorld(View* view);

		void renderView(View* view);
		void renderSky(View* view, SkyMeshComponent* skyMesh);
		void setupLights(GraphicsWorld* graphicsWorld);
	
		virtual void bindMaterialForMesh(MeshComponent* mesh, Material* material, IMaterialInstance* materialInstance) = 0;

		virtual void renderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh) = 0;
		virtual void renderShadows(View* view) = 0;

		virtual void takeScreenshot() = 0;
		
		virtual void clearScreen() = 0;
		virtual void clearRenderTarget( IRenderTarget* renderTarget ) = 0;
	 
		virtual void setSwapChainRenderTarget() = 0;

		// Too simple but i dont regret
		void setRenderMode(RendererViewMode mode) { m_currentViewMode = mode; }
		RendererViewMode getRenderMode() { return m_currentViewMode; }

		void toggleShowingWireframe();
		void toggleShowOctree();

		IRenderTarget* getSwapChainRenderTarget() { return m_swapChainRenderTarget; }

		void renderLoadscreen();

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

	std::weak_ptr<Material> getDefaultMaterial();

	class ISamplerState;
	extern ISamplerState* g_defaultSampler;

}

#endif // !RENDERER_H
