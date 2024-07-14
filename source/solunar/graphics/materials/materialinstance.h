#ifndef GRAPHICS_MATERIALS_MATERIALINSTANCE_H
#define GRAPHICS_MATERIALS_MATERIALINSTANCE_H

namespace engine
{

enum VertexFactory
{
	VertexFactory_StaticMesh,
	VertexFactory_SkinnedMesh,
	VertexFactory_Billboard,
	VertexFactory_Count
};

enum PixelVariations
{
	PixelVariation_Unlit = 1 << 0,
	PixelVariation_Lit = 1 << 1,
};

class IShaderProgram;

class MaterialInstance
{
public:
	virtual ~MaterialInstance();

	// #TODO: Old deprecated API
	virtual IShaderProgram* getStaticMeshShaderProgram() = 0;
	virtual IShaderProgram* getSkinnedShaderProgram() = 0;
	virtual IShaderProgram* getParticleShaderProgram() = 0;
};

}

#endif // !GRAPHICS_MATERIALS_MATERIALINSTANCE_H
