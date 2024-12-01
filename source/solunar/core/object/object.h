#ifndef OBJECT_H
#define OBJECT_H

#include <memory>

namespace solunar
{
	typedef void (*StaticConstructor_t)(void* ptr);

	class TypeInfo
	{
	public:
		TypeInfo(const char* name, StaticConstructor_t staticConstructor, size_t classSize, size_t classAlign, const TypeInfo* baseInfo);
		~TypeInfo();

		const char* GetEntityClassName() const { return m_name; }
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
		static void StaticConstructor(void* ptr) { reinterpret_cast<typeName*>(ptr)->typeName::typeName(); } \
		static const TypeInfo* GetStaticTypeInfo() { return &typeName::s_typeInfo; } \
		virtual const TypeInfo* GetTypeInfo() { return GetStaticTypeInfo();  }

#define IMPLEMENT_ROOT_OBJECT(typeName) \
	const TypeInfo typeName::s_typeInfo(#typeName, typeName::StaticConstructor, sizeof(typeName), alignof(typeName), nullptr)

#define IMPLEMENT_OBJECT(typeName, baseTypeName) \
	const TypeInfo typeName::s_typeInfo(#typeName, typeName::StaticConstructor, sizeof(typeName), alignof(typeName), baseTypeName::GetStaticTypeInfo())
	
#define OBJECT_GET_TYPEINFO(typeName) \
	typeName::GetStaticTypeInfo()

	class Object
	{
		DECLARE_OBJECT(Object);
	public:
		Object();
		virtual ~Object();

		bool IsExactly(const TypeInfo* classTypeInfo)
		{
			return (GetTypeInfo()->GetStringHash() == classTypeInfo->GetStringHash());
		}

		bool IsA(const TypeInfo* classTypeInfo)
		{
			for (const TypeInfo* typeInfo = GetTypeInfo(); typeInfo != nullptr; typeInfo = typeInfo->m_baseInfo)
				if (typeInfo->m_stringHash == classTypeInfo->m_stringHash)
					return true;

			return false;
		}

		template <typename T>
		bool IsExactly()
		{
			return (GetTypeInfo()->GetStringHash() == T::GetStaticTypeInfo()->GetStringHash());
		}

		template <typename T>
		bool IsA()
		{
			for (const TypeInfo* typeInfo = GetTypeInfo(); typeInfo != nullptr; typeInfo = typeInfo->m_baseInfo)
				if (typeInfo->m_stringHash == T::GetStaticTypeInfo()->m_stringHash)
					return true;

			return false;
		}

	};
	
	template <typename T>
	T* dynamicCast(Object* object)
	{
		if (object->IsA<T>())
			return (T*)object;
		
		return nullptr;
	}
	
	template <typename T>
	std::shared_ptr<T> dynamicCastPtr(const std::shared_ptr<Object>& object)
	{
		if (object->IsA<T>())
			return std::static_pointer_cast<T>(object);
		
		return std::shared_ptr<T>();
	}

	template <typename T, typename U>
	std::shared_ptr<T> dynamicCastPtr(const std::shared_ptr<U>& object)
	{
		if (object->IsA<T>())
		{
			return std::static_pointer_cast<T>(object);
		}

		return std::shared_ptr<T>();
	}

	template <typename T>
	std::weak_ptr<T> dynamicCastPtr(const std::shared_ptr<Object>& object)
	{
		if (object->IsA<T>())
			return std::static_pointer_cast<T>(object);

		return std::weak_ptr<T>();
	}

	template <typename T>
	std::weak_ptr<T> dynamicCastPtr(const std::weak_ptr<Object>& object)
	{
		if (std::shared_ptr<Object> objectPtr = object.lock())
		{
			if (objectPtr->IsA<T>())
			{		
				return std::static_pointer_cast<T>(objectPtr);
			}
		}

		return std::weak_ptr<T>();
	}

	template <typename T, typename U>
	std::weak_ptr<T> dynamicCastWeakPtr(const std::weak_ptr<U>& object)
	{
		if (std::shared_ptr<U> objectPtr = object.lock())
		{
			if (objectPtr->IsA<T>())
			{
				return std::static_pointer_cast<T>(objectPtr);
			}
		}

		return std::weak_ptr<T>();
	}

	void ObjectDeleter(Object* p);
}

#endif // !OBJECT_H
