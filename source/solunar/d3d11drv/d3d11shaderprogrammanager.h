#ifndef D3D11SHADERPROGRAMMANAGER_H
#define D3D11SHADERPROGRAMMANAGER_H

#include "graphics/ShaderProgramManager.h"

namespace solunar
{

class D3D11ShaderProgramManager : public ShaderProgramManager
{
public:
	D3D11ShaderProgramManager();
	~D3D11ShaderProgramManager();

	IShaderProgram* CreateShaderProgram(const char* vsfilename, 
										const char* fsfilename, 
										const char* defines = nullptr, 
										InputLayoutDesc* inputLayout = nullptr, 
										int inputLayoutCount = 0) override;

	void SetShaderProgram(IShaderProgram* program) override;
	void DeleteProgram(IShaderProgram* program) override;

private:
	std::vector<IShaderProgram*> m_cachedPrograms;

};

}

#endif // !D3D11SHADERPROGRAMMANAGER_H
