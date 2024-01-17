#include "d3d11drv_pch.h"
#include "d3d11drv/d3d11shaderprogrammanager.h"
#include "d3d11drv/d3d11shaderprogram.h"
#include "d3d11drv/d3d11device.h"

#include "core/file/filesystem.h"
#include "core/file/contentmanager.h"
namespace engine
{

std::string loadShaderText(const std::string& filename)
{
	std::string content;

	DataStreamPtr f = g_contentManager->openStream(filename);

	f->seek(Seek_End, 0);
	size_t fileLength = f->tell();
	f->seek(Seek_Begin, 0);

	content.resize(fileLength + 1);
	f->read((void*)content.data(), fileLength);
	content[fileLength] = '\0';

	f = nullptr;

	return content;
}

D3D11ShaderProgramManager::D3D11ShaderProgramManager()
{
}

D3D11ShaderProgramManager::~D3D11ShaderProgramManager()
{
	for (auto it : m_cachedPrograms)
	{
		if (it)
		{
			mem_delete(it);
			it = nullptr;
		}
	}

	m_cachedPrograms.clear();
}

IShaderProgram* D3D11ShaderProgramManager::createShaderProgram(const char* vsfilename, const char* fsfilename, const char* defines)
{
	Assert(this);

	D3D11Device* device = (D3D11Device*)g_renderDevice;

	Core::msg("[d3d11drv]: compile shader program from files %s, %s", vsfilename, fsfilename);

	std::string vspath = m_shaderPath;
	vspath += "/";
	vspath += vsfilename;

	std::string pspath = m_shaderPath;
	pspath += "/";
	pspath += fsfilename;

	std::string vstext = loadShaderText(vspath);
	std::string pstext = loadShaderText(pspath);

	// ignore defines :(

	D3D11ShaderProgram* shaderProgram = mem_new<D3D11ShaderProgram>(device, vstext.c_str(), pstext.c_str());
	m_cachedPrograms.push_back(shaderProgram);
	return shaderProgram;
}

void D3D11ShaderProgramManager::setShaderProgram(IShaderProgram* program)
{
	D3D11Device* device = (D3D11Device*)g_renderDevice;
	D3D11ShaderProgram* nativeProgram = (D3D11ShaderProgram*)program;

	// #TODO: REFACTOR
	if (nativeProgram)
	{
		device->getDeviceContext()->IASetInputLayout(nativeProgram->getInputLayout());
		device->getDeviceContext()->VSSetShader(nativeProgram->getVertexShader(), NULL, 0);
		device->getDeviceContext()->PSSetShader(nativeProgram->getPixelShader(), NULL, 0);
	}
	else
	{
		device->getDeviceContext()->IASetInputLayout(NULL);
		device->getDeviceContext()->VSSetShader(NULL, NULL, 0);
		device->getDeviceContext()->PSSetShader(NULL, NULL, 0);
	}
}

void D3D11ShaderProgramManager::deleteProgram(IShaderProgram* program)
{
}

}