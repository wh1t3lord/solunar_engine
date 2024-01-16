#include "core/memory/memorymanager.h"
#include "core/core.h"
#include "core/object/typemanager.h"
#include "core/object/object.h"

#include <string.h>

namespace engine
{

namespace
{
	static TypeManager s_typeManager;

	Object* createObjectPrivate(const TypeInfo* typeInfo)
	{
		IAllocator* pAllocator = MemoryManager::getDefaultAllocator();
		Object* pObject = (Object*)pAllocator->allocate(typeInfo->getClassSize(), typeInfo->getClassAlign());

		StaticConstructor_t objectConstructor = typeInfo->getStaticConstructor();
		Assert3(objectConstructor, "Failed to create object without static constructor", typeInfo->getClassName());

		objectConstructor(pObject);

		return pObject;
	}
}

TypeManager* TypeManager::getInstance()
{
	return &s_typeManager;
}

const TypeInfo* TypeManager::getTypeInfoByName(const char* name)
{
	for (auto it : m_registeredTypes)
	{
		if (strcmp(it->getClassName(), name) == 0)
			return it;
	}

	return nullptr;
}

const TypeInfo* TypeManager::getTypeInfoById(size_t hash)
{
	for (auto it : m_registeredTypes)
	{
		if (it->getStringHash() == hash)
			return it;
	}

	return nullptr;
}

Object* TypeManager::createObjectByName(const char* name)
{
	for (auto it : m_registeredTypes)
	{
		if (strcmp(it->m_name, name) == 0)
			return createObjectPrivate(it);
	}

	return nullptr;
}

Object* TypeManager::createObjectByTypeInfo(const TypeInfo* typeInfo)
{
	for (auto it : m_registeredTypes)
	{
		if (it->isAFast(typeInfo))
			return createObjectPrivate(it);
	}

	return nullptr;
}

void TypeManager::registerType(const TypeInfo* typeInfo)
{
	Core::msg("TypeManager: registered type %s", typeInfo->getClassName());
	m_registeredTypes.push_back(typeInfo);
}

}