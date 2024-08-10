#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

namespace solunar
{
	
class ShaderProgramManager;

class IShaderProgram
{
	friend class ShaderProgramManager;
public:
	virtual ~IShaderProgram();

};

}

#endif