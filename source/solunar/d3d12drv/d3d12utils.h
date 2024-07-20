#ifndef D3D12UTILS_H
#define D3D12UTILS_H

namespace engine
{
	bool d3d12ErrorBackend(HRESULT hr, const char* expression, const char* filename, int line, const char* function);
}

#define D3D12_CHECK( STUFF )	\
	{							\
		HRESULT hr = STUFF;		\
		if (FAILED(hr))			\
			d3d12ErrorBackend(hr, #STUFF, __FILE__, __LINE__, __FUNCTION__);	\
	}

#endif // !D3D12UTILS_H
