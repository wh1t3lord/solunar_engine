#ifndef VIEWSURFACE_H
#define VIEWSURFACE_H

namespace engine {
	
	// \brief View Surface interface, surface which display engine world.
	class IViewSurface
	{
	public:
		virtual ~IViewSurface() {}

		// Display everything. (frameSyncNum will be ignored at offscreen surfaces.)
		virtual void display( int frameSyncNum ) = 0;
	};
	
}

#endif