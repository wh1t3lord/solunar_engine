#ifndef DATASTREAM_H
#define DATASTREAM_H

#include "core/file/filesystem.h"

#include <memory>

namespace solunar
{
	class DataStream
	{
	public:
		DataStream();
		virtual ~DataStream();

		virtual size_t Read(void* buffer, size_t count);
		virtual size_t Write(void* buffer, size_t count);

		virtual void Seek(SeekWay dir, long offset);
		virtual long Tell();

		virtual bool Eof();

		virtual void Flush();

		template <typename T>
		size_t Read(T* buffer) { return Read((void*)buffer, sizeof(T)); }

		template <typename T>
		size_t Write(T* buffer) { return Write((void*)buffer, sizeof(T)); }
	};

	typedef std::shared_ptr<DataStream> DataStreamPtr;
}

#endif // DATASTREAM_H
