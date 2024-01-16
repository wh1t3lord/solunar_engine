#ifndef GRAPHICS_MATERIALS_MATERIALINSTANCE_MASKED_H
#define GRAPHICS_MATERIALS_MATERIALINSTANCE_MASKED_H

#include "graphics/materials/materialinstance.h"

namespace engine
{

class MaterialInstance_Masked : public MaterialInstance
{
public:
	MaterialInstance_Masked();
	~MaterialInstance_Masked();

	IShaderProgram* getStaticMeshShaderProgram() override;
	IShaderProgram* getSkinnedShaderProgram() override;
	IShaderProgram* getParticleShaderProgram() override;

};

}

#endif // !GRAPHICS_MATERIALS_MATERIALINSTANCE_MASKED_H
