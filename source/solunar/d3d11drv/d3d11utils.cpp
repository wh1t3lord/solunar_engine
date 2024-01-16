#include "d3d11drv_pch.h"
#include "d3d11drv/d3d11utils.h"

namespace engine
{

bool d3d11ErrorBackend(HRESULT hr, const char* expression, const char* filename, int line, const char* function)
{
	char messageBuffer[1024];

	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		messageBuffer, sizeof(messageBuffer), NULL);

	Debug::assertionFailed(expression, filename, line, function, "D3D11 Assertion Failed", messageBuffer);
	return true;
}

}
