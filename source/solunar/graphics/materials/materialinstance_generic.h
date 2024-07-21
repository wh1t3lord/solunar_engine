#ifndef GRAPHICS_MATERIALS_MATERIALINSTANCE_GENERIC_H
#define GRAPHICS_MATERIALS_MATERIALINSTANCE_GENERIC_H

#include "graphics/materials/materialinstance.h"

namespace engine
{

class IShaderProgram;

class MaterialInstance_Generic : public IMaterialInstance
{
public:
	MaterialInstance_Generic();
	~MaterialInstance_Generic();

	IShaderProgram* getShaderProgramVariation(VertexFactory vertexFactory, uint32_t pixelVariation);

private:
	

};

}

#endif // !GRAPHICS_MATERIALS_MATERIALINSTANCE_GENERIC_H
