#include "core/core.h"
#include "core/platform/core_platform.h"
#include "core/utils/common.h"
#include "core/debug.h"
#include <memory>
#include "core/file/datastream.h"

namespace solunar
{
	DataStream::DataStream()
	{

	}

	DataStream::~DataStream()
	{

	}

	size_t DataStream::Read(void* buffer, size_t count)
	{
		return 0;
	}

	size_t DataStream::Write(void* buffer, size_t count)
	{
		return 0;
	}

	void DataStream::Seek(SeekWay dir, long offset)
	{

	}

	long DataStream::Tell()
	{
		return 0;
	}

	bool DataStream::Eof()
	{
		return false;
	}

	void DataStream::Flush()
	{

	}
}
