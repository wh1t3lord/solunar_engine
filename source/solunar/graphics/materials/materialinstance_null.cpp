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

IShaderProgram* MaterialInstance_Null::getStaticMeshShaderProgram()
{
    return nullptr;
}

IShaderProgram* MaterialInstance_Null::getSkinnedShaderProgram()
{
    return nullptr;
}

IShaderProgram* MaterialInstance_Null::getParticleShaderProgram()
{
    return nullptr;
}

}