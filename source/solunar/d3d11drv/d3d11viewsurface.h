#ifndef D3D11VIEWSURFACE_H
#define D3D11VIEWSURFACE_H

#include "graphics/viewsurface.h"

namespace engine
{
	class D3D11ViewSurface : public IViewSurface
	{
	public:
		D3D11ViewSurface(void* handle);
		~D3D11ViewSurface();

		void display(int frameSyncNum) override;

	private:
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11Texture2D* m_depthStencilTexture;
		ID3D11DepthStencilView* m_depthStencilView;
		IDXGISwapChain* m_swapChain;
		void* m_windowHandle;
	};
}


#endif // !D3D11VIEWSURFACE_H
