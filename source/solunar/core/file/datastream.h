#ifndef DATASTREAM_H
#define DATASTREAM_H

#include "core/file/filesystem.h"

#include <memory>

namespace engine
{
	class CORE_API DataStream
	{
	public:
		DataStream();
		virtual ~DataStream();

		virtual size_t read(void* buffer, size_t count);
		virtual size_t write(void* buffer, size_t count);

		virtual void seek(SeekWay dir, long offset);
		virtual long tell();

		virtual bool eof();

		virtual void flush();

		template <typename T>
		size_t read(T* buffer) { return read((void*)buffer, sizeof(T)); }

		template <typename T>
		size_t write(T* buffer) { return write((void*)buffer, sizeof(T)); }
	};

	typedef std::shared_ptr<DataStream> DataStreamPtr;
}

#endif // DATASTREAM_H
