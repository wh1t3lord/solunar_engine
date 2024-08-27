#ifndef CORE_FILESYSTEM_H
#define CORE_FILESYSTEM_H

#include <stddef.h>

namespace solunar
{

enum SeekWay
{
	Seek_Begin,
	Seek_Current,
	Seek_End
};

typedef void* FileHandle;

//! Abstract class for file system.
class FileSystem
{
public:
	virtual ~FileSystem() {}

	//! Open file.
	virtual FileHandle Open(const char* filename) = 0;

	//! Create file.
	virtual FileHandle Create(const char* filename) = 0;
	
	//! Close file
	virtual void Close(FileHandle handle) = 0;

	//! File exist.
	virtual bool Exist(const char* filename) = 0;

	virtual size_t Tell(FileHandle handle) = 0;
	virtual void Seek(FileHandle handle, SeekWay seekway, long offset) = 0;

	virtual size_t Read(FileHandle handle, void* data, size_t size) = 0;
	virtual size_t Write(FileHandle handle, const void* data, size_t size) = 0;
};

extern FileSystem* g_fileSystem;

}

#endif // !CORE_FILESYSTEM_H
