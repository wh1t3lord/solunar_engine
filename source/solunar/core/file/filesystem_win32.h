#ifndef CORE_FILESYSTEM_WIN32_H
#define CORE_FILESYSTEM_WIN32_H

#include "core/file/filesystem.h"

namespace solunar
{

class FileSystem_Win32 : public FileSystem
{
public:
	FileSystem_Win32();
	~FileSystem_Win32();

	//! Open file.
	FileHandle Open(const char* filename);

	//! Create file.
	FileHandle Create(const char* filename);

	//! Close file
	void Close(FileHandle handle);

	//! File exist.
	bool Exist(const char* filename);

	size_t Tell(FileHandle handle);
	void Seek(FileHandle handle, SeekWay seekway, long offset);

	size_t Read(FileHandle handle, void* data, size_t size);
	size_t Write(FileHandle handle, const void* data, size_t size);
};

}

#endif // !CORE_FILESYSTEM_WIN32_H
