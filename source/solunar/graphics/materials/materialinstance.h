#ifndef GRAPHICS_MATERIALS_MATERIALINSTANCE_H
#define GRAPHICS_MATERIALS_MATERIALINSTANCE_H

namespace solunar
{

enum VertexFactory
{
	VertexFactory_StaticMesh,
	VertexFactory_SkinnedMesh,
	VertexFactory_Billboard,
	VertexFactory_Count
};

enum PixelVariation
{
	PixelVariation_Unlit = 1 << 0,
	PixelVariation_Lit = 1 << 1,
};

class IShaderProgram;

// \brief Interface for material instance.
class IMaterialInstance
{
public:
	virtual ~IMaterialInstance();

	// \brief Get shader program for specific vertex factory and pixel variation(see PixelVariation)
	virtual IShaderProgram* getShaderProgramVariation(VertexFactory vertexFactory, uint32_t pixelVariation) = 0;
};

}

#endif // !GRAPHICS_MATERIALS_MATERIALINSTANCE_H
