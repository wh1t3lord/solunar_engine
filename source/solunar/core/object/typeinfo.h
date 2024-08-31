#ifndef TYPEINFO_H
#define TYPEINFO_H

namespace solunar
{

// Forward declaration
class Object;

// Object creation procedure
typedef void (*StaticConstructor_t)(void* ptr);

class TypeInfo
{
public:
	TypeInfo(const char* name, StaticConstructor_t staticConstructor, size_t classSize, size_t classAlign, const TypeInfo* baseInfo);
	~TypeInfo();

	const char* GetClassName() const { return m_name; }
	const size_t GetStringHash() const { return m_stringHash; }

	const StaticConstructor_t GetStaticConstructor() const { return m_staticConstructor; }
	const size_t GetClassSize() const { return  m_classSize; }
	const size_t GetClassAlign() const { return  m_classAlign; }

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

public:
	const char* m_name;
	StaticConstructor_t m_staticConstructor;
	size_t m_classSize;
	size_t m_classAlign;

	const TypeInfo* m_baseInfo;
	size_t m_stringHash;

};

#define DECLARE_OBJECT(typeName) \
	public: \
		static const TypeInfo s_typeInfo; \
		static void StaticConstructor(void* ptr) { new(ptr) typeName(); } \
		static const TypeInfo* GetStaticTypeInfo() { return &typeName::s_typeInfo; } \
		virtual const TypeInfo* GetTypeInfo() { return GetStaticTypeInfo();  }

#define DECLARE_OBJECT_INTERFACE(typeName) \
	public: \
		static const TypeInfo s_typeInfo; \
		static void StaticConstructor(void* ptr) { } \
		static const TypeInfo* GetStaticTypeInfo() { return &typeName::s_typeInfo; } \
		virtual const TypeInfo* GetTypeInfo() { return GetStaticTypeInfo();  }

#define IMPLEMENT_ROOT_OBJECT(typeName) \
	const TypeInfo typeName::s_typeInfo(#typeName, typeName::StaticConstructor, sizeof(typeName), alignof(typeName), nullptr)

#define IMPLEMENT_OBJECT(typeName, baseTypeName) \
	const TypeInfo typeName::s_typeInfo(#typeName, typeName::StaticConstructor, sizeof(typeName), alignof(typeName), baseTypeName::GetStaticTypeInfo())

#define OBJECT_GET_TYPEINFO(typeName) \
	typeName::GetStaticTypeInfo()

}

#endif // !TYPEINFO_H
