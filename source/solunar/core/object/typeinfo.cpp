#include "core/platform/core_platform.h"
#include "core/debug.h"
#include "core/utils/crc32.h"
#include "core/object/typeinfo.h"

namespace solunar
{

TypeInfo::TypeInfo(const char* name, createObject_t createObjectProc, const TypeInfo* baseInfo)
{
	m_name = name;
	m_createObjectProc = createObjectProc;
	m_baseInfo = baseInfo;
	//m_stringHash = std::hash<std::string>{}(name);
	m_stringHash = (size_t)crc32(1, name, strlen(name));
}

TypeInfo::~TypeInfo()
{
	m_stringHash = 0;
	m_baseInfo = nullptr;
	m_createObjectProc = nullptr;
	m_name = nullptr;
}

const bool TypeInfo::IsA(const TypeInfo* typeInfo) const
{
	Assert(typeInfo);

	for (const TypeInfo* it = this; it != nullptr; it = it->GetBaseInfo())
		if (it->GetStringHash() == typeInfo->GetStringHash())
			return true;

	return false;
}

const bool TypeInfo::IsExactly(const TypeInfo* typeInfo) const
{
	Assert(typeInfo);

	return GetStringHash() == typeInfo->GetStringHash();
}

}