#include "core/core.h"
#include "core/platform/core_platform.h"
#include "core/utils/common.h"
#include "core/debug.h"
#include <memory>
#include "core/file/datastream.h"

namespace engine
{
	DataStream::DataStream()
	{

	}

	DataStream::~DataStream()
	{

	}

	size_t DataStream::read(void* buffer, size_t count)
	{
		return 0;
	}

	size_t DataStream::write(void* buffer, size_t count)
	{
		return 0;
	}

	void DataStream::seek(SeekWay dir, long offset)
	{

	}

	long DataStream::tell()
	{
		return 0;
	}

	bool DataStream::eof()
	{
		return false;
	}

	void DataStream::flush()
	{

	}
}
