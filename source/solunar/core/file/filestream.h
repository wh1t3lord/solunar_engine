#ifndef FILESTREAM_H
#define FILESTREAM_H

#include "core/file/filesystem.h"
#include "core/file/datastream.h"

#include <string>

namespace engine
{
	class FileStream : public DataStream
	{
	public:
		FileStream(const std::string& filename);
		~FileStream();

		size_t read(void* buffer, size_t count) override;
		size_t write(void* buffer, size_t count) override;

		void seek(SeekWay dir, long offset) override;
		long tell() override;

		bool eof() override;

		void flush() override;

	private:
		FileHandle m_fileHandle;
	};
}

#endif // FILESTREAM_H
