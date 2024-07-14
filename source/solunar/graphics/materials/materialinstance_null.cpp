#include "graphicspch.h"
#include "graphics/materials/materialinstance_null.h"

namespace engine
{

MaterialInstance_Null::MaterialInstance_Null()
{
}

MaterialInstance_Null::~MaterialInstance_Null()
{
}

IShaderProgram* MaterialInstance_Null::getShaderProgramVariation(VertexFactory vertexFactory, uint32_t pixelVariation)
{
	return nullptr;
}

}