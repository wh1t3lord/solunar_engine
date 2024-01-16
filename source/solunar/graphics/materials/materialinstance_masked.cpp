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

IShaderProgram* MaterialInstance_Masked::getStaticMeshShaderProgram()
{
    return nullptr;
}

IShaderProgram* MaterialInstance_Masked::getSkinnedShaderProgram()
{
    return nullptr;
}

IShaderProgram* MaterialInstance_Masked::getParticleShaderProgram()
{
    return nullptr;
}

}