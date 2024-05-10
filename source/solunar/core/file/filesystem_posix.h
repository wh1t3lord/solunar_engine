#ifndef CORE_FILESYSTEM_POSIX_H
#define CORE_FILESYSTEM_POSIX_H

#include "core/file/filesystem.h"

namespace engine
{

class FileSystem_Posix : public FileSystem
{
public:
	FileSystem_Posix();
	~FileSystem_Posix();

	//! Open file.
	FileHandle open(const char* filename);

	//! Create file.
	FileHandle create(const char* filename);

	//! Close file
	void close(FileHandle handle);

	//! File exist.
	bool exist(const char* filename);

	size_t tell(FileHandle handle);
	void seek(FileHandle handle, SeekWay seekway, long offset);

	size_t read(FileHandle handle, void* data, size_t size);
	size_t write(FileHandle handle, const void* data, size_t size);
};

}

#endif // !CORE_FILESYSTEM_POSIX_H
