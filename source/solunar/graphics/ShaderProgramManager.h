#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

namespace engine
{
	
class IShaderProgram;

class IShaderProgramManager
{
public:
	virtual ~IShaderProgramManager() {}

	void init(const char* shaderPath);
	void shutdown();

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

extern IShaderProgramManager* g_shaderManager;
	
}

#endif
