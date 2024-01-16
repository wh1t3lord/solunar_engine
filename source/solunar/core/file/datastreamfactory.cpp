#include "core/core.h"
#include "core/debug.h"
#include "core/memory/memorymanager.h"
#include "core/file/datastreamfactory.h"

namespace engine
{

namespace
{
	DataStreamFactory* g_dataStreamFactory = nullptr;
}

void DataStreamFactory::createInstance()
{
	Assert(!g_dataStreamFactory);
	g_dataStreamFactory = mem_new<DataStreamFactory>();
}

void DataStreamFactory::destroyInstance()
{
	Assert(g_dataStreamFactory);

	mem_delete(g_dataStreamFactory);
	g_dataStreamFactory = nullptr;
}

DataStreamFactory::DataStreamFactory()
{
}

DataStreamFactory::~DataStreamFactory()
{
}

}