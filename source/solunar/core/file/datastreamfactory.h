#ifndef CORE_FILE_DATASTREAMFACTORY_H
#define CORE_FILE_DATASTREAMFACTORY_H

#include "core/utils/singleton.h"
#include "core/file/datastream.h"

namespace engine
{

class CORE_API DataStreamFactory : public Singleton<DataStreamFactory>
{
public:
	static void createInstance();
	static void destroyInstance();

public:
	DataStreamFactory();
	~DataStreamFactory();
};

}

#endif // !CORE_FILE_DATASTREAMFACTORY_H
