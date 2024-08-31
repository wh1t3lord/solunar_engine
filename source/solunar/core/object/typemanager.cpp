#include "core/memory/memorymanager.h"
#include "core/core.h"
#include "core/object/typemanager.h"
#include "core/object/object.h"

#include <string.h>

namespace solunar
{

static TypeManager s_typeManager;
TypeManager* g_typeManager = &s_typeManager;

const TypeInfo* TypeManager::GetTypeInfoByName(const char* name)
{
	for (auto it : m_registeredTypes)
	{
		if (strcmp(it->GetClassName(), name) == 0)
			return it;
	}

	return nullptr;
}

const TypeInfo* TypeManager::GetTypeInfoById(size_t hash)
{
	for (auto it : m_registeredTypes)
	{
		if (it->GetStringHash() == hash)
			return it;
	}

	return nullptr;
}

Object* TypeManager::CreateObjectByName(const char* name)
{
	for (auto it : m_registeredTypes)
	{
		if (strcmp(it->GetClassName(), name) == 0)
		{
			createObject_t createObjectProc = it->GetCreateObjectProc();
			return createObjectProc();
		}
	}

	return nullptr;
}

Object* TypeManager::CreateObjectByTypeInfo(const TypeInfo* typeInfo)
{
	for (auto it : m_registeredTypes)
	{
		if (it->IsExactly(typeInfo))
		{
			createObject_t createObjectProc = it->GetCreateObjectProc();
			return createObjectProc();
		}
	}

	return nullptr;
}

void TypeManager::RegisterType(const TypeInfo* typeInfo)
{
	Core::Msg("TypeManager: registered type %s", typeInfo->GetClassName());
	m_registeredTypes.push_back(typeInfo);
}

void TypeManager::GetRegisteredTypes(std::vector<const TypeInfo*>& registeredTypes)
{
	registeredTypes = m_registeredTypes;
}

}