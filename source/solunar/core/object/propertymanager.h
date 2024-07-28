#ifndef PROPERTYMANAGER_H
#define PROPERTYMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "core/utils/singleton.h"
#include "core/math/math_types.h"
#include "core/math/boundingbox.h"
#include "core/object/object.h"

namespace engine
{

enum PropertyType
{
	PropertyType_Unknown,
	PropertyType_Integer,
	PropertyType_Float,
	PropertyType_Vector2,
	PropertyType_Vector3,
	PropertyType_Vector4,
	PropertyType_Quaternion,
	PropertyType_Matrix4x4,
	PropertyType_BoundingBox,
	PropertyType_String,
	PropertyType_ObjectPointer,
	PropertyType_Count
};

class IProperty
{
public:
	virtual ~IProperty() {}

	virtual PropertyType getType() = 0;
	virtual const char* getName() = 0;
	virtual size_t getOffset() = 0;
};

template <typename T, PropertyType Type>
class PropertyImpl : public IProperty
{
public:
	PropertyImpl(const char* name, size_t offset);
	~PropertyImpl();

	PropertyType getType() override;
	const char* getName() override;
	size_t getOffset() override;

private:
	const char* m_name;
	size_t m_offset;
};

template<typename T, PropertyType Type>
inline PropertyImpl<T, Type>::PropertyImpl(const char* name, size_t offset) :
	m_name(name),
	m_offset(offset)
{
}

template<typename T, PropertyType Type>
inline PropertyImpl<T, Type>::~PropertyImpl()
{
	m_name = nullptr;
}

template<typename T, PropertyType Type>
inline PropertyType PropertyImpl<T, Type>::getType()
{
	return Type;
}

template<typename T, PropertyType Type>
inline const char* PropertyImpl<T, Type>::getName()
{
	return m_name;
}

template<typename T, PropertyType Type>
inline size_t PropertyImpl<T, Type>::getOffset()
{
	return m_offset;
}

template <typename T>
void PropertyGetValue(Object* object, IProperty* property, T& value);

template<typename T>
inline void PropertyGetValue(Object* object, IProperty* property, T& value)
{
	void* data = object + property->getOffset();
	T* propertyData = reinterpret_cast<T*>(data);
	value = *propertyData;
}

template<>
inline void PropertyGetValue(Object* object, IProperty* property, std::string& value)
{
	Assert(property->getType() == PropertyType_String);
	std::string* stringData = (std::string*)(((char*)object) + property->getOffset());
	value = *stringData;
}

// typedefing 
using PropertyInt = PropertyImpl<int, PropertyType_Integer>;
using PropertyFloat = PropertyImpl<int, PropertyType_Float>;
using PropertyString = PropertyImpl<std::string, PropertyType_String>;
using PropertyVector2 = PropertyImpl<glm::vec2, PropertyType_Vector2>;
using PropertyVector3 = PropertyImpl<glm::vec3, PropertyType_Vector3>;
using PropertyVector4 = PropertyImpl<glm::vec4, PropertyType_Vector4>;
using PropertyQuaternion = PropertyImpl<glm::quat, PropertyType_Quaternion>;
using PropertyMatrix4 = PropertyImpl<glm::quat, PropertyType_Matrix4x4>;
using PropertyBoundingBox = PropertyImpl<BoundingBox, PropertyType_BoundingBox>;

// Macro for more beautiful registration
#define RegisterProperty(className, propertyType, propertyName) \
	PropertyManager::getInstance()->registerProperty(className::getStaticTypeInfo(), mem_new<propertyType>(#propertyName, offsetof(className, propertyName)));

// \brief Property Manager - managing properties list for each TypeInfo 
class PropertyManager : public Singleton<PropertyManager>
{
private:
	static PropertyManager ms_instance;

public:
	PropertyManager();
	~PropertyManager();

	void shutdown();

	void registerProperty(const TypeInfo* typeInfo, IProperty* propertyInstance);

	IProperty* findProperty(const TypeInfo* typeInfo, const char* name);

	void getTypeProperties(const TypeInfo* typeInfo, std::vector<IProperty*>& properties);

private:
	std::unordered_map<size_t, std::vector<IProperty*>> m_properies;
};

typedef void (*RegisterPropertiesFunc)();

// \brief PropertyRegistrator - Singleton which register class properties.
class PropertyRegistrator
{
public:
	static PropertyRegistrator* getInstance();

public:
	PropertyRegistrator();
	~PropertyRegistrator();

	void addFunc(RegisterPropertiesFunc func);

	void registerClasses();

private:
	std::vector<RegisterPropertiesFunc> m_funcs;
};

#define DeclarePropertyRegister(className) \
	static void registerProperties();

#define BeginPropertyRegister(className) \
	void className::registerProperties()
	
#define EndPropertyRegister(className) \
	static struct Registrator##className##Props { \
		Registrator##className##Props() { \
			PropertyRegistrator::getInstance()->addFunc(className::registerProperties); \
		} \
	} g_registrator##className##Props;

}

#endif // !PROPERTYMANAGER_H
