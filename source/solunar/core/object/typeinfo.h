#ifndef TYPEINFO_H
#define TYPEINFO_H

#include "core/memory/memorymanager.h"

namespace solunar
{

// Forward declaration
class Object;

// Object creation procedure
typedef Object* (*createObject_t)();

class TypeInfo
{
public:
	TypeInfo(const char* name, createObject_t createObjectProc, const TypeInfo* baseInfo);
	~TypeInfo();

	const char* GetClassName() const { return m_name; }
	const size_t GetStringHash() const { return m_stringHash; }
	const TypeInfo* GetBaseInfo() const { return m_baseInfo; }
	const createObject_t GetCreateObjectProc() const { return m_createObjectProc; }

	const bool IsA(const TypeInfo* typeInfo) const;
	const bool IsExactly(const TypeInfo* typeInfo) const;

	template <typename T>
	const bool IsA() const
	{
		return IsA(T::GetStaticTypeInfo());
	}

	template <typename T>
	const bool IsExactly() const
	{
		return IsExactly(T::GetStaticTypeInfo());
	}

private:
	const char* m_name;
	const TypeInfo* m_baseInfo;

	createObject_t m_createObjectProc;

	size_t m_stringHash;
};

#define DECLARE_OBJECT(typeName) \
	public: \
		static const TypeInfo s_typeInfo; \
		static Object* CreateObject() { return (Object*)mem_new<typeName>(); } \
		static const TypeInfo* GetStaticTypeInfo() { return &typeName::s_typeInfo; } \
		virtual const TypeInfo* GetTypeInfo() { return GetStaticTypeInfo();  }

#define DECLARE_OBJECT_INTERFACE(typeName) \
	public: \
		static const TypeInfo s_typeInfo; \
		static Object* CreateObject() { return nullptr; } \
		static const TypeInfo* GetStaticTypeInfo() { return &typeName::s_typeInfo; } \
		virtual const TypeInfo* GetTypeInfo() { return GetStaticTypeInfo();  }

#define IMPLEMENT_ROOT_OBJECT(typeName) \
	const TypeInfo typeName::s_typeInfo(#typeName, typeName::CreateObject, nullptr)

#define IMPLEMENT_OBJECT(typeName, baseTypeName) \
	const TypeInfo typeName::s_typeInfo(#typeName, typeName::CreateObject, baseTypeName::GetStaticTypeInfo())


#define OBJECT_GET_TYPEINFO(typeName) \
	typeName::GetStaticTypeInfo()

}

#endif // !TYPEINFO_H
