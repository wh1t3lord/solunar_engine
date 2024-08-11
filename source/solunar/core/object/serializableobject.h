#ifndef SERIALIZABLEOBJECT_H
#define SERIALIZABLEOBJECT_H

#include <tinyxml2.h>

#include "core/math/math_types.h"
#include "core/file/datastream.h"
#include "core/object/object.h"

namespace solunar
{
	class SerializableObject : public Object
	{
	public:
		IMPLEMENT_OBJECT(SerializableObject, Object);
	
	public:
		SerializableObject();
		virtual ~SerializableObject();

		virtual void Load(const std::shared_ptr<DataStream>& stream);
		virtual void Save(const std::shared_ptr<DataStream>& stream);

		virtual void LoadXML(tinyxml2::XMLElement& element);
		virtual void SaveXML(tinyxml2::XMLElement& element);
	};

	glm::vec3 GetVector3FromXMLElement(tinyxml2::XMLElement& element);
	void SaveVector3ToXMLElement(tinyxml2::XMLElement& element, const glm::vec3& v);
}

#endif