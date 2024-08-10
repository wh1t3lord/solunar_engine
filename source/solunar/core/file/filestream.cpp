#include "core/utils/common.h"
#include "core/debug.h"
#include "core/file/filestream.h"

#include <string>

namespace solunar
{
	FileStream::FileStream(const std::string& filename) :
		m_fileHandle(nullptr)
	{
		m_fileHandle = g_fileSystem->open(filename.c_str());
		Assert3(m_fileHandle, "Failed to open file", filename.c_str());
	}

	FileStream::~FileStream()
	{
		if (m_fileHandle)
		{
			g_fileSystem->close(m_fileHandle);
			m_fileHandle = nullptr;
		}
	}

	size_t FileStream::read(void* buffer, size_t count)
	{
		return g_fileSystem->read(m_fileHandle, buffer, count);
	}

	size_t FileStream::write(void* buffer, size_t count)
	{
		return g_fileSystem->write(m_fileHandle, buffer, count);
	}

	void FileStream::seek(SeekWay dir, long offset)
	{
		g_fileSystem->seek(m_fileHandle, dir, offset);
	}

	long FileStream::tell()
	{
		return g_fileSystem->tell(m_fileHandle);
	}

	bool FileStream::eof()
	{
		Assert2(0, "eof() is unimplemented");
		return false;
	}

	void FileStream::flush()
	{
		Assert2(0, "flush() is unimplemented");
	}

}
