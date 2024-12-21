#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

namespace solunar
{

enum ShaderType
{
	ShaderType_Vertex,
	ShaderType_Pixel
};

class ShaderProgramManager;

class IShaderProgram
{
	friend class ShaderProgramManager;
public:
	virtual ~IShaderProgram();

};

}

#endif