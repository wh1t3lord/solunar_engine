#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

namespace engine
{

class IRenderDevice;
class IViewSurface;

// \brief Base interface for any renderer visual.
class IRenderVisual
{
public:
	virtual ~IRenderVisual() {}

	// Render function
	virtual void render(const glm::mat4& matrix) = 0;
};

// \brief Renderer class.
class Renderer
{
public:
	Renderer();
	~Renderer();

	// Initialize renderer
	void init();

	// Shutdown renderer
	void shutdown();

	// Start begin of rendering frame.
	void beginFrame();

	// End of rendering frame.
	void endFrame();

	// Render the view surface. (frameSyncNum will be ignored at offscreen surfaces.)
	void renderView(IViewSurface* view, int frameSyncNum);

	// Create view surface from window.
	IViewSurface* createViewSurface(void* handle);

	// Get render device.
	IRenderDevice* getRenderDevice();

};

extern Renderer* g_renderer;

}

#endif // !GRAPHICS_RENDERER_H
