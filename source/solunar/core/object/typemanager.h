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
	static TypeManager* getInstance();

public:
	const TypeInfo* getTypeInfoByName(const char* name);
	const TypeInfo* getTypeInfoById(size_t hash);

	Object* createObjectByName(const char* name);
	Object* createObjectByTypeInfo(const TypeInfo* typeInfo);

	template <typename T>
	T* createObject();

	void registerType(const TypeInfo* typeInfo);

	template <typename T>
	void registerObject();

	void getRegisteredTypes(std::vector<const TypeInfo*>& registeredTypes);

private:
	std::vector<const TypeInfo*> m_registeredTypes;
};

template<typename T>
inline T* TypeManager::createObject()
{
	return (T*)createObjectByTypeInfo(T::getStaticTypeInfo());
}

template<typename T>
inline void TypeManager::registerObject()
{
	registerType( T::getStaticTypeInfo() );
}

#define g_typeManager TypeManager::getInstance()

}

#endif // !CORE_REFLECTION_TYPEMANAGER_H
