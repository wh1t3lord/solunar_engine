#include "graphicspch.h"
#include "graphics/shaderprogram.h"
#include "graphics/ShaderProgramManager.h"

namespace engine
{

ShaderProgramManager* g_shaderManager = nullptr;

void ShaderProgramManager::init(const char* shaderPath)
{
	Assert2(this, "Please initialize shader manager for exactly graphics driver API.");
	Assert2(shaderPath, "Please specify shader path for exactly graphics driver API.");

	m_shaderPath = shaderPath;
}

void ShaderProgramManager::shutdown()
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
