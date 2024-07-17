#ifndef GLSHADERPROGRAM_H
#define GLSHADERPROGRAM_H

#include "graphics/shaderprogram.h"

namespace engine
{
	
class GLShaderProgram : public IShaderProgram
{
public:
	GLShaderProgram(const std::string& vsfilename, const std::string& fsfilename);
	GLShaderProgram(const std::string& vsfilename, const std::string& fsfilename, const char* defines);

	~GLShaderProgram() override;

	GLuint getProgramhandle();

private:
	std::string m_file_name;
	std::string m_defines;
	GLuint m_program;
};

}

#endif // !GLSHADERPROGRAM_H
