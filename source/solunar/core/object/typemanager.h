#ifndef CORE_OBJECT_TYPEMANAGER_H
#define CORE_OBJECT_TYPEMANAGER_H

#include <vector>

#include "core/object/object.h"

namespace solunar
{

class TypeInfo;
class ITypedObject;

class TypeManager
{
public:
	const TypeInfo* GetTypeInfoByName(const char* name);
	const TypeInfo* GetTypeInfoById(size_t hash);

	Object* CreateObjectByName(const char* name);
	Object* CreateObjectByTypeInfo(const TypeInfo* typeInfo);

	void RegisterType(const TypeInfo* typeInfo);

	void GetRegisteredTypes(std::vector<const TypeInfo*>& registeredTypes);

	template <typename T>
	T* CreateObject();

	template <typename T>
	void RegisterObject();

private:
	std::vector<const TypeInfo*> m_registeredTypes;
};

template<typename T>
inline T* TypeManager::CreateObject()
{
	return (T*)CreateObjectByTypeInfo(T::GetStaticTypeInfo());
}

template<typename T>
inline void TypeManager::RegisterObject()
{
	RegisterType( T::GetStaticTypeInfo() );
}

extern TypeManager* g_typeManager;

}

#endif // !CORE_REFLECTION_TYPEMANAGER_H
