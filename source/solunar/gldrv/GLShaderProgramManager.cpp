#include "gldrv_pch.h"
#include "gldrv/GLShaderProgramManager.h"
#include "gldrv/glshaderprogram.h"
#include <glad/glad.h>
namespace engine
{

GLShaderProgramManager::GLShaderProgramManager()
{
}

GLShaderProgramManager::~GLShaderProgramManager()
{
}

IShaderProgram* GLShaderProgramManager::createShaderProgram(
	const char* vsfilename, 
	const char* fsfilename, 
	const char* defines,
	InputLayoutDesc* inputLayout,
	int inputLayoutCount)
{
	//if (defines)
	//{
	//	for (std::vector<IShaderProgram*>::iterator it = m_programs.begin(); it != m_programs.end(); ++it) {
	//		if ((*it)->m_file_name == name && !(*it)->m_defines.empty() && (*it)->m_defines == defines) {
	//			return (*it);
	//		}
	//	}
	//}
	//else
	//{
	//	for (std::vector<IShaderProgram*>::iterator it = m_programs.begin(); it != m_programs.end(); ++it) {
	//		if ((*it)->m_file_name == name && !(*it)->m_defines.empty()) {
	//			return (*it);
	//		}
	//	}
	//}

	std::string vspath = m_shaderPath;
	vspath += "/";
	vspath += vsfilename;

	std::string pspath = m_shaderPath;
	pspath += "/";
	pspath += fsfilename;

	GLShaderProgram* program = mem_new<GLShaderProgram>(vspath, pspath, defines);
	//m_programs.push_back(program);
	return (GLShaderProgram*)program;
}

void GLShaderProgramManager::setShaderProgram(IShaderProgram* program)
{
	GLShaderProgram* nativeProgram = (GLShaderProgram*)program;
	glUseProgram(nativeProgram ? nativeProgram->getProgramhandle() : 0);
}

void GLShaderProgramManager::deleteProgram(IShaderProgram* program)
{
	if (program) {
		mem_delete(program);
	}
}

}