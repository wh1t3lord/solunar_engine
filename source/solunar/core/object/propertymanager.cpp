#include "core/core.h"
#include "core/debug.h"
#include "core/memory/memorymanager.h"
#include "core/object/propertymanager.h"

namespace engine
{

// Global instance
PropertyManager PropertyManager::ms_instance;

PropertyManager::PropertyManager()
{
}

PropertyManager::~PropertyManager()
{
}

void PropertyManager::shutdown()
{
	for (auto it : m_properies) {
		for (auto prop : it.second) {
			mem_delete(prop);
		}
	}

	m_properies.clear();
}

void PropertyManager::registerProperty(const TypeInfo* typeInfo, IProperty* propertyInstance)
{
	Assert(typeInfo);
	Assert(propertyInstance);

	// try to find vector
	auto it = m_properies.find(typeInfo->getStringHash());
	if (it == m_properies.end())
	{
		std::vector<IProperty*> properties;
		properties.push_back(propertyInstance);
		m_properies.emplace(typeInfo->getStringHash(), properties);
	}
	else
	{
		m_properies.find(typeInfo->getStringHash())->second.push_back(propertyInstance);
	}

	Core::msg("PropertyManager: registered property %s for type %s", propertyInstance->getName(), typeInfo->getClassName());
}

IProperty* PropertyManager::findProperty(const TypeInfo* typeInfo, const char* name)
{
	Assert(m_properies.find(typeInfo->getStringHash()) != m_properies.end());

	auto it = m_properies.find(typeInfo->getStringHash());
	for (auto prop : it->second)
	{
		if (strcmp(prop->getName(), name) == 0)
			return prop;
	}

	return nullptr;
}

void PropertyManager::getTypeProperties(const TypeInfo* typeInfo, std::vector<IProperty*>& properties)
{
	auto it = m_properies.find(typeInfo->getStringHash());
	if (it != m_properies.end()) {
		properties = it->second;
	}
}

PropertyRegistrator* PropertyRegistrator::getInstance()
{
	static PropertyRegistrator s_propertyRegistrator;
	return &s_propertyRegistrator;
}

PropertyRegistrator::PropertyRegistrator()
{
}

PropertyRegistrator::~PropertyRegistrator()
{
}

void PropertyRegistrator::addFunc(RegisterPropertiesFunc func)
{
	m_funcs.push_back(func);
}

void PropertyRegistrator::registerClasses()
{
	for (auto it : m_funcs)
		it();

	m_funcs.clear();
}

}