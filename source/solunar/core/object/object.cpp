#include "core/core.h"
#include "core/debug.h"
#include "core/memory/memorymanager.h"
#include <memory>

#include "core/object/object.h"

#include <string>
#include <functional>

namespace solunar
{

IMPLEMENT_ROOT_OBJECT(Object);

Object::Object()
{
}

Object::~Object()
{
}

void ObjectDeleter(Object* p)
{
	mem_delete(p);
	p = nullptr;
}

}
