#ifndef D3D11UTILS_H
#define D3D11UTILS_H

namespace solunar
{
	bool d3d11ErrorBackend(HRESULT hr, const char* expression, const char* filename, int line, const char* function);
}

#define D3D11_CHECK( STUFF )	\
	{							\
		HRESULT hr = STUFF;		\
		if (FAILED(hr))			\
			d3d11ErrorBackend(hr, #STUFF, __FILE__, __LINE__, __FUNCTION__);	\
	}

#endif // !D3D11UTILS_H
