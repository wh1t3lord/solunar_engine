#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

namespace engine
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