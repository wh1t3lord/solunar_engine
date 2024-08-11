#ifndef GLSHADERPROGRAM_H
#define GLSHADERPROGRAM_H

#include "graphics/shaderprogram.h"

#include <array>

namespace solunar
{
	
class GLShaderProgram : public IShaderProgram
{
public:
	GLShaderProgram(const std::string& vsfilename, const std::string& fsfilename, InputLayoutDesc* inputLayout = nullptr, int inputLayoutCount = 0);
	GLShaderProgram(const std::string& vsfilename, const std::string& fsfilename, const char* defines, InputLayoutDesc* inputLayout = nullptr, int inputLayoutCount = 0);

	~GLShaderProgram() override;

	GLuint getProgramhandle();

	const std::vector<InputLayoutDesc>& getInputLayout();

private:
	//std::array<InputLayoutDesc, 16> m_inputLayout;
	std::vector<InputLayoutDesc> m_inputLayout;
	std::string m_file_name;
	std::string m_defines;
	GLuint m_program;
};

}

#endif // !GLSHADERPROGRAM_H
