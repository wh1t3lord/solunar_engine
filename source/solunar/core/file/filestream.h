#ifndef FILESTREAM_H
#define FILESTREAM_H

#include "core/file/filesystem.h"
#include "core/file/datastream.h"

#include <string>

namespace solunar
{
	class FileStream : public DataStream
	{
	public:
		FileStream(const std::string& filename);
		~FileStream();

		size_t Read(void* buffer, size_t count) override;
		size_t Write(void* buffer, size_t count) override;

		void Seek(SeekWay dir, long offset) override;
		long Tell() override;

		bool Eof() override;

		void Flush() override;

	private:
		FileHandle m_fileHandle;
	};
}

#endif // FILESTREAM_H
