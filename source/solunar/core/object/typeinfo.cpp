#include "core/platform/core_platform.h"
#include "core/debug.h"
#include "core/utils/crc32.h"
#include "core/object/typeinfo.h"

namespace solunar
{

TypeInfo::TypeInfo(const char* name, StaticConstructor_t staticConstructor, size_t classSize, size_t classAlign, const TypeInfo* baseInfo)
{
	m_name = name;
	m_staticConstructor = staticConstructor;
	m_classSize = classSize;
	m_classAlign = classAlign;
	m_baseInfo = baseInfo;
	m_stringHash = (size_t)crc32(1, name, strlen(name));
	//m_stringHash = std::hash<std::string>{}(name);
}

TypeInfo::~TypeInfo()
{
	m_baseInfo = nullptr;
	m_name = nullptr;
}

const bool TypeInfo::IsA(const TypeInfo* typeInfo) const
{
	Assert(typeInfo);

	for (const TypeInfo* it = this; it != nullptr; it = it->m_baseInfo)
		if (it->GetStringHash() == typeInfo->GetStringHash())
			return true;

	return false;
}

const bool TypeInfo::IsExactly(const TypeInfo* typeInfo) const
{
	Assert(typeInfo);
	return (GetStringHash() == typeInfo->GetStringHash());
}

}