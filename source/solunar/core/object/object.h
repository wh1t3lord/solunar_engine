#ifndef OBJECT_H
#define OBJECT_H

#include "core/object/typeinfo.h"

#include <memory>

namespace solunar
{

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
			for (const TypeInfo* typeInfo = GetTypeInfo(); typeInfo != nullptr; typeInfo = typeInfo->GetBaseInfo())
				if (typeInfo->GetStringHash() == classTypeInfo->GetStringHash())
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
			for (const TypeInfo* typeInfo = GetTypeInfo(); typeInfo != nullptr; typeInfo = typeInfo->GetBaseInfo())
				if (typeInfo->GetStringHash() == T::GetStaticTypeInfo()->GetStringHash())
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
