#include "graphicspch.h"
#include "graphics/materials/materialinstance_masked.h"

namespace engine
{

MaterialInstance_Masked::MaterialInstance_Masked()
{
}

MaterialInstance_Masked::~MaterialInstance_Masked()
{
}

IShaderProgram* MaterialInstance_Masked::getShaderProgramVariation(VertexFactory vertexFactory, uint32_t pixelVariation)
{
    return nullptr;
}

}