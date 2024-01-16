#ifndef SERIALIZABLEOBJECT_H
#define SERIALIZABLEOBJECT_H

#include <tinyxml2.h>

#include "core/math/math_types.h"
#include "core/file/datastream.h"
#include "core/object/object.h"

namespace engine
{
	class SerializableObject : public Object
	{
	public:
		ImplementObject(SerializableObject, Object);
	
	public:
		SerializableObject();
		virtual ~SerializableObject();

		virtual void load(const std::shared_ptr<DataStream>& stream);
		virtual void save(const std::shared_ptr<DataStream>& stream);

		virtual void loadXML(tinyxml2::XMLElement& element);
		virtual void saveXML(tinyxml2::XMLElement& element);
	};

	glm::vec3 getVector3FromXMLElement(tinyxml2::XMLElement& element);
	void saveVector3ToXMLElement(tinyxml2::XMLElement& element, const glm::vec3& v);
}

#endif