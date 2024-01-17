#ifndef GRAPHICS_MATERIALS_MATERIALINSTANCE_H
#define GRAPHICS_MATERIALS_MATERIALINSTANCE_H

namespace engine
{

enum VertexFactory
{
};

class IShaderProgram;

class MaterialInstance
{
public:
	virtual ~MaterialInstance();

	virtual IShaderProgram* getStaticMeshShaderProgram() = 0;
	virtual IShaderProgram* getSkinnedShaderProgram() = 0;
	virtual IShaderProgram* getParticleShaderProgram() = 0;

};

}

#endif // !GRAPHICS_MATERIALS_MATERIALINSTANCE_H
