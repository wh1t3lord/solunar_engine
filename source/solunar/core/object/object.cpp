#include "core/core.h"
#include "core/debug.h"
#include "core/memory/memorymanager.h"
#include <memory>

#include "core/object/object.h"

#include <string>
#include <functional>

namespace engine
{
	TypeInfo::TypeInfo(const char* name, StaticConstructor_t staticConstructor, size_t classSize, size_t classAlign, const TypeInfo* baseInfo)
	{
		m_name = name;
		m_staticConstructor = staticConstructor;
		m_classSize = classSize;
		m_classAlign = classAlign;
		m_baseInfo = baseInfo;
		m_stringHash = std::hash<std::string>{}(name);
	}

	TypeInfo::~TypeInfo()
	{
		m_baseInfo = nullptr;
		m_name = nullptr;
	}

	const bool TypeInfo::isA(const TypeInfo* typeInfo) const
	{
		Assert(typeInfo);

		for (const TypeInfo* it = this; it != nullptr; it = it->m_baseInfo)
			if (it->getStringHash() == typeInfo->getStringHash())
				return true;

		return false;
	}

	const bool TypeInfo::isExactly(const TypeInfo* typeInfo) const
	{
		Assert(typeInfo);
		return (getStringHash() == typeInfo->getStringHash());
	}


	Object::Object()
	{
	}

	Object::~Object()
	{
	}

	void objectDeleter(Object* p)
	{
		mem_delete(p);
		p = nullptr;
	}

}
