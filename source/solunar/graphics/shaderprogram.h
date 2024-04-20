#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

namespace engine
{
	
class IShaderProgramManager;

class IShaderProgram
{
	friend class IShaderProgramManager;
public:
	virtual ~IShaderProgram();

};

}

#endif