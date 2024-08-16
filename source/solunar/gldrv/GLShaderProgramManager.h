#ifndef GLSHADERMANAGER_H
#define GLSHADERMANAGER_H

#include "graphics/ShaderProgramManager.h"

namespace solunar
{

class GLShaderProgramManager : public ShaderProgramManager
{
public:
	GLShaderProgramManager();
	~GLShaderProgramManager();

	IShaderProgram* CreateShaderProgram(
		const char* vsfilename, 
		const char* fsfilename,
		const char* defines = nullptr,
		InputLayoutDesc* inputLayout = nullptr,
		int inputLayoutCount = 0) override;

	void SetShaderProgram(IShaderProgram* program) override;
	void DeleteProgram(IShaderProgram* program) override;

private:
	std::vector<IShaderProgram*> m_programs;
};

}

#endif
