#include "core/utils/common.h"
#include "core/debug.h"
#include "core/file/filestream.h"

#include <string>

namespace solunar
{
	FileStream::FileStream(const std::string& filename) :
		m_fileHandle(nullptr)
	{
		m_fileHandle = g_fileSystem->Open(filename.c_str());
		Assert3(m_fileHandle, "Failed to open file", filename.c_str());
	}

	FileStream::~FileStream()
	{
		if (m_fileHandle)
		{
			g_fileSystem->Close(m_fileHandle);
			m_fileHandle = nullptr;
		}
	}

	size_t FileStream::Read(void* buffer, size_t count)
	{
		return g_fileSystem->Read(m_fileHandle, buffer, count);
	}

	size_t FileStream::Write(void* buffer, size_t count)
	{
		return g_fileSystem->Write(m_fileHandle, buffer, count);
	}

	void FileStream::Seek(SeekWay dir, long offset)
	{
		g_fileSystem->Seek(m_fileHandle, dir, offset);
	}

	long FileStream::Tell()
	{
		return g_fileSystem->Tell(m_fileHandle);
	}

	bool FileStream::Eof()
	{
		Assert2(0, "Eof() is unimplemented");
		return false;
	}

	void FileStream::Flush()
	{
		Assert2(0, "Flush() is unimplemented");
	}

}
