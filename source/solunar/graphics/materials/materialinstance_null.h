#ifndef GRAPHICS_MATERIALS_MATERIALINSTANCE_NULL_H
#define GRAPHICS_MATERIALS_MATERIALINSTANCE_NULL_H

#include "graphics/materials/materialinstance.h"

namespace solunar
{

class MaterialInstance_Null : public IMaterialInstance
{
public:
	MaterialInstance_Null();
	~MaterialInstance_Null();

	// Inherited via IMaterialInstance
	IShaderProgram* getShaderProgramVariation(VertexFactory vertexFactory, uint32_t pixelVariation) override;
};

}

#endif // !GRAPHICS_MATERIALS_MATERIALINSTANCE_GENERIC_H
