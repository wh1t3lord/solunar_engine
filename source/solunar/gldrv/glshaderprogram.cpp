#include "gldrv_pch.h"
#include "gldrv/glshaderprogram.h"
#include "core/file/contentmanager.h"

namespace solunar
{

GLuint createShader(GLenum target, const char* filename, const char* defines = nullptr)
{
	std::string content;

	DataStreamPtr f = g_contentManager->OpenStream(filename);

	f->Seek(Seek_End, 0);
	size_t fileLength = f->Tell();
	f->Seek(Seek_Begin, 0);

	content.resize(fileLength + 1);
	f->Read((void*)content.data(), fileLength);
	content[fileLength] = '\0';

	if (defines && strlen(defines) > 0) {
		size_t versionLocation = content.find("#version 330 core");

		size_t definesStringLength = versionLocation + strlen("#version 330 core") + 2;
		std::string newContentString = content.substr(0, definesStringLength);

		newContentString.append("#define ");
		newContentString.append(defines);
		newContentString.append(content.substr(definesStringLength));

		content = newContentString;
	}

	const char* contentCStr = content.c_str();

	GLuint shader = glCreateShader(target);
	glShaderSource(shader, 1, &contentCStr, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		Core::Msg("=== SHADER COMPILE ERROR ===");
		Core::Msg("%s", contentCStr);
		Core::Msg("============================");
		Core::Error("createShader: failed to compile shader %s\n%s", filename, infoLog);
	}

	Core::Msg("[graphics]: created %s shader from file %s", (target == GL_VERTEX_SHADER) ? "vertex" : "fragment", filename);

	return shader;
}

GLShaderProgram::GLShaderProgram(const std::string& vsfilename, const std::string& fsfilename, InputLayoutDesc* inputLayout /*= nullptr*/, int inputLayoutCount /*= 0*/)
{
	m_defines = "";

	GLuint vertex_shader = createShader(GL_VERTEX_SHADER, vsfilename.c_str());
	GLuint fragment_shader = createShader(GL_FRAGMENT_SHADER, fsfilename.c_str());

	m_program = glCreateProgram();
	glAttachShader(m_program, vertex_shader);
	glAttachShader(m_program, fragment_shader);
	glLinkProgram(m_program);

	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);

	int  success;
	char infoLog[512];
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_program, 512, NULL, infoLog);
		Core::Error("IShaderProgram::IShaderProgram: failed to link program %s", infoLog);
	}

	m_inputLayout.resize(inputLayoutCount);
	memcpy(m_inputLayout.data(), inputLayout, inputLayoutCount * sizeof(InputLayoutDesc));
}

GLShaderProgram::GLShaderProgram(const std::string& vsfilename, const std::string& fsfilename, const char* defines, InputLayoutDesc* inputLayout /*= nullptr*/, int inputLayoutCount /*= 0*/)
{
	m_defines = defines ? defines : "";

	GLuint vertex_shader = createShader(GL_VERTEX_SHADER, vsfilename.c_str(), defines);
	GLuint fragment_shader = createShader(GL_FRAGMENT_SHADER, fsfilename.c_str(), defines);

	m_program = glCreateProgram();
	glAttachShader(m_program, vertex_shader);
	glAttachShader(m_program, fragment_shader);
	glLinkProgram(m_program);

	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);

	int  success;
	char infoLog[512];
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_program, 512, NULL, infoLog);
		Core::Error("IShaderProgram::IShaderProgram: failed to link program %s", infoLog);
	}

	m_inputLayout.resize(inputLayoutCount);
	memcpy(m_inputLayout.data(), inputLayout, inputLayoutCount * sizeof(InputLayoutDesc));
}

GLShaderProgram::~GLShaderProgram()
{
	glDeleteProgram(m_program);
}

GLuint GLShaderProgram::getProgramhandle()
{
	return m_program;
}

const std::vector<InputLayoutDesc>& GLShaderProgram::getInputLayout()
{
	return m_inputLayout;
}

}