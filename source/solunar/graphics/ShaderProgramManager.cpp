#include "graphicspch.h"
#include "graphics/shaderprogram.h"
#include "graphics/ShaderProgramManager.h"

namespace solunar
{

ShaderProgramManager* g_shaderManager = nullptr;

void ShaderProgramManager::Init(const char* shaderPath)
{
	Assert2(this, "Please Initialize shader manager for exactly graphics driver API.");
	Assert2(shaderPath, "Please specify shader path for exactly graphics driver API.");

	m_shaderPath = shaderPath;
}

void ShaderProgramManager::Shutdown()
{
	//for (auto it = m_programs.begin(); it != m_programs.end(); ++it) {
	//	if (*it) {
	//		deleteProgram(*it);
	//	}
	//}

	//m_programs.clear();
}

std::string ShaderProgramManager::getShaderPath()
{
	return m_shaderPath;
}
	
}
