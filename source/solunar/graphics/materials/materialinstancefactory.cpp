#include "graphicspch.h"
#include "graphics/materials/materialinstancefactory.h"
#include "graphics/materials/materialinstance_generic.h"
#include "graphics/materials/materialinstance_masked.h"
#include "graphics/materials/materialinstance_null.h"

#include "graphics/material.h"

namespace solunar
{

MaterialInstanceFactory* MaterialInstanceFactory::ms_instance = nullptr;

MaterialInstanceFactory* MaterialInstanceFactory::createInstance()
{
	Assert2(!ms_instance, "Material factory is already created");

	ms_instance = mem_new<MaterialInstanceFactory>();
	return ms_instance;
}

void MaterialInstanceFactory::destroyInstance()
{
	Assert2(ms_instance, "Material factory is not created");

	mem_delete(ms_instance);
	ms_instance = nullptr;
}

IMaterialInstance* MaterialInstanceFactory::createMaterialInstance(Material* material)
{
	Assert(material);

	IMaterialInstance* materialInstance = nullptr;
	if (material->getMaterialType() == MaterialType::Generic)
		materialInstance = (IMaterialInstance*)mem_new<MaterialInstance_Generic>();
	else if (material->getMaterialType() == MaterialType::Masked)
		materialInstance = (IMaterialInstance*)mem_new<MaterialInstance_Masked>();
	else
		materialInstance = (IMaterialInstance*)mem_new<MaterialInstance_Null>();
	
	return materialInstance;
}

}