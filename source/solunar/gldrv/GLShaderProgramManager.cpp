#include "gldrv_pch.h"
#include "gldrv/GLShaderProgramManager.h"
#include "gldrv/glshaderprogram.h"
#include <glad/glad.h>
namespace engine
{

size_t g_vertexAttribsSizeTable[(size_t)ImageFormat::LAST] =
{
	3, // RGB16
	4, // RGBA16
	3, // RGB32
	4, // RGBA32
	SIZE_MAX,// DEPTH24 NOT USED
	SIZE_MAX,// DEPTH32 NOT USED
	1, // R32
	1, // R32F
	3, // RGB16F
	4, // RGBA16F
	2, // RG32F
	3, // RGB32F
	4, // RGBA32F
	SIZE_MAX,// DEPTH32F NOT USED
	4, // RGBA32_UNORM
};

GLShaderProgramManager::GLShaderProgramManager()
{
}

GLShaderProgramManager::~GLShaderProgramManager()
{
	for (auto it : m_programs)
	{
		if (it)
		{
			mem_delete(it);
			it = nullptr;
		}
	}

	m_programs.clear();
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

	GLShaderProgram* program = mem_new<GLShaderProgram>(vspath, pspath, defines, inputLayout, inputLayoutCount);
	m_programs.push_back(program);
	return (GLShaderProgram*)program;
}

void GLShaderProgramManager::setShaderProgram(IShaderProgram* program)
{
	GLShaderProgram* nativeProgram = (GLShaderProgram*)program;
	glUseProgram(nativeProgram ? nativeProgram->getProgramhandle() : 0);

	// apply vertex format

	const std::vector<InputLayoutDesc>& layouts = nativeProgram->getInputLayout();
	size_t appliedOffset = 0;

	size_t stride = 0;
	
	// calculate stride
	for (int i = 0; i < layouts.size(); i++)
	{
		const InputLayoutDesc& layoutEntry = layouts[i];
		stride += g_vertexAttribsSizeTable[(size_t)layoutEntry.m_format] * sizeof(float);
	}

	for (int i = 0; i < layouts.size(); i++)
	{
		const InputLayoutDesc& layoutEntry = layouts[i];

		glEnableVertexAttribArray(GLuint(i));
		glVertexAttribPointer(GLuint(i), GLint(g_vertexAttribsSizeTable[(size_t)layoutEntry.m_format]),
			GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride),
			(appliedOffset > 0) ? (void*)(appliedOffset * sizeof(float)) : (void*)0);

		appliedOffset += g_vertexAttribsSizeTable[(size_t)layoutEntry.m_format];
	}
}

void GLShaderProgramManager::deleteProgram(IShaderProgram* program)
{
	Assert(program);

	if (program) {
		mem_delete(program);
	}
}

}