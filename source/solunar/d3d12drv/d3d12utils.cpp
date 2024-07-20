#include "d3d12drv_pch.h"
#include "d3d12drv/d3d12utils.h"

namespace engine
{

bool d3d12ErrorBackend(HRESULT hr, const char* expression, const char* filename, int line, const char* function)
{
	char messageBuffer[1024];

	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		messageBuffer, sizeof(messageBuffer), NULL);

	Debug::assertionFailed(expression, filename, line, function, "D3D12 Assertion Failed", messageBuffer);
	return true;
}

}
