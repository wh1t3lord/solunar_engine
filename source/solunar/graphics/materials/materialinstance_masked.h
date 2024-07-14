#ifndef GRAPHICS_MATERIALS_MATERIALINSTANCE_MASKED_H
#define GRAPHICS_MATERIALS_MATERIALINSTANCE_MASKED_H

#include "graphics/materials/materialinstance.h"

namespace engine
{

class MaterialInstance_Masked : public IMaterialInstance
{
public:
	MaterialInstance_Masked();
	~MaterialInstance_Masked();

	// Inherited via IMaterialInstance
	IShaderProgram* getShaderProgramVariation(VertexFactory vertexFactory, uint32_t pixelVariation) override;

};

}

#endif // !GRAPHICS_MATERIALS_MATERIALINSTANCE_MASKED_H
