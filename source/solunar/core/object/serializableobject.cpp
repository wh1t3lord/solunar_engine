#include "core/object/serializableobject.h"

namespace solunar
{
	SerializableObject::SerializableObject()
	{
	}

	SerializableObject::~SerializableObject()
	{
	}

	void SerializableObject::Load(const std::shared_ptr<DataStream>& stream)
	{
	}

	void SerializableObject::Save(const std::shared_ptr<DataStream>& stream)
	{
	}

	void SerializableObject::LoadXML(tinyxml2::XMLElement& element)
	{
	}

	void SerializableObject::SaveXML(tinyxml2::XMLElement& element)
	{
	}

	glm::vec3 GetVector3FromXMLElement(tinyxml2::XMLElement& element)
	{
		glm::vec3 v(0.0f);

		const tinyxml2::XMLAttribute* attrX = element.FindAttribute("x");
		if (attrX)
			v.x = attrX->FloatValue();

		const tinyxml2::XMLAttribute* attrY = element.FindAttribute("y");
		if (attrY)
			v.y = attrY->FloatValue();

		const tinyxml2::XMLAttribute* attrZ = element.FindAttribute("z");
		if (attrZ)
			v.z = attrZ->FloatValue();

		return v;
	}

	void SaveVector3ToXMLElement(tinyxml2::XMLElement& element, const glm::vec3& v)
	{
		element.SetAttribute("x", v.x);
		element.SetAttribute("y", v.y);
		element.SetAttribute("z", v.z);
	}

}