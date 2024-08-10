#ifndef CORE_FILESYSTEM_H
#define CORE_FILESYSTEM_H

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
	virtual FileHandle open(const char* filename) = 0;

	//! Create file.
	virtual FileHandle create(const char* filename) = 0;
	
	//! Close file
	virtual void close(FileHandle handle) = 0;

	//! File exist.
	virtual bool exist(const char* filename) = 0;

	virtual size_t tell(FileHandle handle) = 0;
	virtual void seek(FileHandle handle, SeekWay seekway, long offset) = 0;

	virtual size_t read(FileHandle handle, void* data, size_t size) = 0;
	virtual size_t write(FileHandle handle, const void* data, size_t size) = 0;
};

extern FileSystem* g_fileSystem;

}

#endif // !CORE_FILESYSTEM_H
