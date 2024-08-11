#ifndef GRAPHICS_MATERIALS_MATERIALINSTANCEFACTORY_H
#define GRAPHICS_MATERIALS_MATERIALINSTANCEFACTORY_H

#include "graphics/material.h"

namespace solunar
{

class Material;
class IMaterialInstance;

class MaterialInstanceFactory : public Singleton<MaterialInstanceFactory>
{
	static MaterialInstanceFactory* ms_instance;
public:
	static MaterialInstanceFactory* CreateInstance();
	static void DestroyInstance();

public:
	IMaterialInstance* createMaterialInstance(Material* material);
};

}

#endif // !GRAPHICS_MATERIALS_MATERIALINSTANCEFACTORY_H
