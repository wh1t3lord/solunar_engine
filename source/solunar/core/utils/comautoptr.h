#ifndef CORE_UTILS_COMAUTOPTR_H
#define CORE_UTILS_COMAUTOPTR_H

#include "core/core.h"

#include <typeinfo>

namespace solunar
{

//! Template wrapper on COM objects.
template <typename T>
class ComAutoPtr
{
	
public:
	ComAutoPtr();
	ComAutoPtr(T* pInstance);
	~ComAutoPtr();

	void addRef();
	void release();

	T* get() { return m_instance; }
	T** getDoublePointer() { return &m_instance; }

	T* operator->() {return m_instance; }
	T* operator()() { return m_instance; }
	T** operator&() { return &m_instance; }

	//ComAutoPtr& operator=(std::nullptr_t)
	//{
	//	release();
	//	return *this;
	//}

	//ComAutoPtr& operator=(T* other)
	//{
	//	T* instance = m_instance;
	//	m_instance = other;

	//	if (m_instance)
	//	{
	//		m_instance->AddRef();
	//	}

	//	if (instance)
	//	{
	//		instance->Release();
	//	}

	//	return *this;
	//}

	//ComAutoPtr& operator=(const ComAutoPtr& other)
	//{
	//	return operator=(other.get());
	//}


private:
	T* m_instance;
};

template<typename T>
inline ComAutoPtr<T>::ComAutoPtr()
{
	m_instance = nullptr;
}

template<typename T>
inline ComAutoPtr<T>::ComAutoPtr(T* instance)
{
	ASSERT(instance);
	m_instance = instance;
}

template<typename T>
inline ComAutoPtr<T>::~ComAutoPtr()
{
	release();
}

template<typename T>
inline void ComAutoPtr<T>::addRef()
{
	if (m_instance)
		m_instance->AddRef();
}

template<typename T>
inline void ComAutoPtr<T>::release()
{
	if (m_instance)
	{
		ULONG refCount = m_instance->Release();

		Core::msg("[%s] releasing with ref count %ul", typeid(this).name(), refCount);
		
		m_instance = nullptr;
	}
}

}

#endif