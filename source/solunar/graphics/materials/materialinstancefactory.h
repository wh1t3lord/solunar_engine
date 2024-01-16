#ifndef GRAPHICS_MATERIALS_MATERIALINSTANCEFACTORY_H
#define GRAPHICS_MATERIALS_MATERIALINSTANCEFACTORY_H

#include "graphics/material.h"

namespace engine
{

class Material;
class MaterialInstance;

class MaterialInstanceFactory : public Singleton<MaterialInstanceFactory>
{
	static MaterialInstanceFactory* ms_instance;
public:
	static MaterialInstanceFactory* createInstance();
	static void destroyInstance();

public:
	MaterialInstance* createMaterialInstance(Material* material);
};

}

#endif // !GRAPHICS_MATERIALS_MATERIALINSTANCEFACTORY_H
