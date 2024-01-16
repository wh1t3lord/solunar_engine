#include "core/object/serializableobject.h"

namespace engine
{
	SerializableObject::SerializableObject()
	{
	}

	SerializableObject::~SerializableObject()
	{
	}

	void SerializableObject::load(const std::shared_ptr<DataStream>& stream)
	{
	}

	void SerializableObject::save(const std::shared_ptr<DataStream>& stream)
	{
	}

	void SerializableObject::loadXML(tinyxml2::XMLElement& element)
	{
	}

	void SerializableObject::saveXML(tinyxml2::XMLElement& element)
	{
	}

	glm::vec3 getVector3FromXMLElement(tinyxml2::XMLElement& element)
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

	void saveVector3ToXMLElement(tinyxml2::XMLElement& element, const glm::vec3& v)
	{
		element.SetAttribute("x", v.x);
		element.SetAttribute("y", v.y);
		element.SetAttribute("z", v.z);
	}

}