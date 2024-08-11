#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

namespace solunar
{
	
class IShaderProgram;

class ShaderProgramManager
{
public:
	virtual ~ShaderProgramManager() {}

	void Init(const char* shaderPath);
	void Shutdown();

	std::string getShaderPath();

	virtual	IShaderProgram* createShaderProgram(const char* vsfilename,
		const char* fsfilename,
		const char* defines = nullptr,
		InputLayoutDesc* inputLayout = nullptr,
		int inputLayoutCount = 0) = 0;

	virtual void setShaderProgram(IShaderProgram* program) = 0;
	virtual void deleteProgram(IShaderProgram* program) = 0;

protected:
	//std::vector<IShaderProgram*> m_programs;
	std::string m_shaderPath;
};

extern ShaderProgramManager* g_shaderManager;
	
}

#endif
