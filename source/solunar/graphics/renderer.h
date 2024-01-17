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
	class MaterialInstance;
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

		virtual void init();
		virtual void shutdown();

	//	void initForView(View* view);
	
		View* getView() { return m_view; }

		virtual void beginFrame();
		virtual void endFrame();

		void renderWorld(View* view);

		void renderView(View* view);
		void renderSky(View* view, SkyMeshComponent* skyMesh);
	
		virtual void bindMaterialForMesh(MeshComponent* mesh, Material* material, MaterialInstance* materialInstance) = 0;

		virtual void renderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh) = 0;
		virtual void renderShadows(View* view) = 0;

		virtual void takeScreenshot() = 0;
		
		virtual void clearScreen() = 0;
	 
		// Too simple but i dont regret
		void setRenderMode(RendererViewMode mode) { m_currentViewMode = mode; }
		RendererViewMode getRenderMode() { return m_currentViewMode; }

		void toggleShowingWireframe();
		void toggleShowOctree();

	protected:
		ITexture2D* m_screenColorTexture;
		ITexture2D* m_screenDepthTexture;
		IRenderTarget* m_screenRenderTarget;
		
		View* m_view;

		RendererViewMode m_currentViewMode;

		bool m_meshPolysWireframe;
		bool m_showOctree;
	};

	extern Renderer* g_renderer;

	void createRenderer();
	void destroyRenderer();

}

#endif // !RENDERER_H
