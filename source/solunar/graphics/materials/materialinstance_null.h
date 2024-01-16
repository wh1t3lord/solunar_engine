#ifndef GRAPHICS_MATERIALS_MATERIALINSTANCE_NULL_H
#define GRAPHICS_MATERIALS_MATERIALINSTANCE_NULL_H

#include "graphics/materials/materialinstance.h"

namespace engine
{

class MaterialInstance_Null : public MaterialInstance
{
public:
	MaterialInstance_Null();
	~MaterialInstance_Null();

	IShaderProgram* getStaticMeshShaderProgram() override;
	IShaderProgram* getSkinnedShaderProgram() override;
	IShaderProgram* getParticleShaderProgram() override;
};

}

#endif // !GRAPHICS_MATERIALS_MATERIALINSTANCE_GENERIC_H
