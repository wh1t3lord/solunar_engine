#ifndef OBJECT_H
#define OBJECT_H

#include <memory>

namespace engine
{
	typedef void (*StaticConstructor_t)(void* ptr);

	class TypeInfo
	{
	public:
		TypeInfo(const char* name, StaticConstructor_t staticConstructor, size_t classSize, size_t classAlign, const TypeInfo* baseInfo);
		~TypeInfo();

		const char* getClassName() const { return m_name; }
		const size_t getStringHash() const { return m_stringHash; }

		const StaticConstructor_t getStaticConstructor() const { return m_staticConstructor; }
		const size_t getClassSize() const { return  m_classSize; }
		const size_t getClassAlign() const { return  m_classAlign; }

		const bool isA(const TypeInfo* typeInfo) const;
		const bool isExactly(const TypeInfo* typeInfo) const;

		template <typename T>
		const bool isA() const
		{
			return isA(T::getStaticTypeInfo());
		}

	public:
		const char* m_name;
		StaticConstructor_t m_staticConstructor;
		size_t m_classSize;
		size_t m_classAlign;

		const TypeInfo* m_baseInfo;
		size_t m_stringHash;

	};


#define ImplementRootObject(typeName) \
	public: \
		static void staticConstructor(void* ptr) { reinterpret_cast<typeName*>(ptr)->typeName::typeName(); } \
		static const TypeInfo* getStaticTypeInfo() { static const TypeInfo s_typeInfo(#typeName, typeName::staticConstructor, sizeof(typeName), alignof(typeName), nullptr); return &s_typeInfo; } \
		virtual const TypeInfo* getTypeInfo() { return getStaticTypeInfo();  }

#define ImplementObject(typeName, baseTypeName) \
	public: \
		static void staticConstructor(void* ptr) { reinterpret_cast<typeName*>(ptr)->typeName::typeName(); } \
		static const TypeInfo* getStaticTypeInfo() { static const TypeInfo s_typeInfo(#typeName, typeName::staticConstructor, sizeof(typeName), alignof(typeName), baseTypeName::getStaticTypeInfo()); return &s_typeInfo; } \
		virtual const TypeInfo* getTypeInfo() { return getStaticTypeInfo();  }
	
#define ObjectGetTypeInfo(typeName) \
	typeName::getStaticTypeInfo()

	class Object
	{
		ImplementRootObject(Object);
	public:
		Object();
		virtual ~Object();

		template <typename T>
		bool isExactly()
		{
			return (getTypeInfo()->getStringHash() == T::getStaticTypeInfo()->getStringHash());
		}

		bool isExactly(const TypeInfo* classTypeInfo)
		{
			return (getTypeInfo()->getStringHash() == classTypeInfo->getStringHash());
		}

		template <typename T>
		bool isA()
		{
			for (const TypeInfo* typeInfo = getTypeInfo(); typeInfo != nullptr; typeInfo = typeInfo->m_baseInfo)
				if (typeInfo->m_stringHash == T::getStaticTypeInfo()->m_stringHash)
					return true;

			return false;
		}

		bool isA(const TypeInfo* classTypeInfo)
		{
			for (const TypeInfo* typeInfo = getTypeInfo(); typeInfo != nullptr; typeInfo = typeInfo->m_baseInfo)
				if (typeInfo->m_stringHash == classTypeInfo->m_stringHash)
					return true;

			return false;
		}
	};
	
	template <typename T>
	T* dynamicCast(Object* object)
	{
		if (object->isA<T>())
			return (T*)object;
		
		return nullptr;
	}
	
	template <typename T>
	std::shared_ptr<T> dynamicCastPtr(const std::shared_ptr<Object>& object)
	{
		if (object->isA<T>())
			return std::static_pointer_cast<T>(object);
		
		return std::shared_ptr<T>();
	}

	template <typename T>
	std::weak_ptr<T> dynamicCastPtr(const std::shared_ptr<Object>& object)
	{
		if (object->isA<T>())
			return std::static_pointer_cast<T>(object);

		return std::weak_ptr<T>();
	}


	template <typename T>
	std::weak_ptr<T> dynamicCastPtr(const std::weak_ptr<Object>& object)
	{
		if (std::shared_ptr<Object> objectPtr = object.lock())
		{
			if (objectPtr->isA<T>())
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
			if (objectPtr->isA<T>())
			{
				return std::static_pointer_cast<T>(objectPtr);
			}
		}

		return std::weak_ptr<T>();
	}

	void objectDeleter(Object* p);
}

#endif // !OBJECT_H
