#ifndef PROPERTYMANAGER_H
#define PROPERTYMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "core/utils/singleton.h"
#include "core/math/math_types.h"
#include "core/math/boundingbox.h"
#include "core/object/object.h"

namespace solunar
{

enum PropertyType
{
	PropertyType_Unknown,
	PropertyType_Bool,
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

	virtual PropertyType GetType() = 0;
	virtual const char* GetName() = 0;
	virtual size_t GetOffset() = 0;
};

template <typename T, PropertyType Type>
class PropertyImpl : public IProperty
{
public:
	PropertyImpl(const char* name, size_t offset);
	~PropertyImpl();

	PropertyType GetType() override;
	const char* GetName() override;
	size_t GetOffset() override;

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
inline PropertyType PropertyImpl<T, Type>::GetType()
{
	return Type;
}

template<typename T, PropertyType Type>
inline const char* PropertyImpl<T, Type>::GetName()
{
	return m_name;
}

template<typename T, PropertyType Type>
inline size_t PropertyImpl<T, Type>::GetOffset()
{
	return m_offset;
}

template <typename T>
void PropertyGetValue(Object* object, IProperty* property, T& value);

template<typename T>
inline void PropertyGetValue(Object* object, IProperty* property, T& value)
{
	void* data = object + property->GetOffset();
	T* propertyData = reinterpret_cast<T*>(data);
	value = *propertyData;
}

template<>
inline void PropertyGetValue(Object* object, IProperty* property, std::string& value)
{
	Assert(property->GetType() == PropertyType_String);
	std::string* stringData = (std::string*)(((char*)object) + property->GetOffset());
	value = *stringData;
}

// typedefing 
using PropertyBool = PropertyImpl<int, PropertyType_Bool>;
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
#define REGISTER_PROPERTY(className, propertyType, propertyName) \
	PropertyManager::GetInstance()->RegisterProperty(className::GetStaticTypeInfo(), mem_new<propertyType>(#propertyName, offsetof(className, propertyName)));

// \brief Property Manager - managing properties list for each TypeInfo 
class PropertyManager : public Singleton<PropertyManager>
{
private:
	static PropertyManager ms_instance;

public:
	PropertyManager();
	~PropertyManager();

	void Shutdown();

	void RegisterProperty(const TypeInfo* typeInfo, IProperty* propertyInstance);

	IProperty* FindProperty(const TypeInfo* typeInfo, const char* name);

	void GetTypeProperties(const TypeInfo* typeInfo, std::vector<IProperty*>& properties);

private:
	std::unordered_map<size_t, std::vector<IProperty*>> m_properies;
};

typedef void (*RegisterPropertiesFunc)();

// \brief PropertyRegistrator - Singleton which register class properties.
class PropertyRegistrator
{
public:
	static PropertyRegistrator* GetInstance();

public:
	PropertyRegistrator();
	~PropertyRegistrator();

	void AddFunc(RegisterPropertiesFunc func);

	void RegisterClasses();

private:
	std::vector<RegisterPropertiesFunc> m_funcs;
};

#define DECLARE_PROPERTY_REGISTER(className) \
	static void RegisterProperties();

#define BEGIN_PROPERTY_REGISTER(className) \
	void className::RegisterProperties()
	
#define END_PROPERTY_REGISTER(className) \
	static struct Registrator##className##Props { \
		Registrator##className##Props() { \
			PropertyRegistrator::GetInstance()->AddFunc(className::RegisterProperties); \
		} \
	} g_registrator##className##Props;

}

#endif // !PROPERTYMANAGER_H
