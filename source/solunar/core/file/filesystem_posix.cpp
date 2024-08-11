#if defined(__unix__) || defined(__unix) || defined(__APPLE__)

#include "core/platform/core_platform.h"
#include "core/file/filesystem_posix.h"

namespace solunar
{

static int s_stdioSeekDir[] = { SEEK_SET, SEEK_CUR, SEEK_END };

FileSystem_Posix::FileSystem_Posix()
{
}

FileSystem_Posix::~FileSystem_Posix()
{
}

FileHandle FileSystem_Posix::open(const char* filename)
{
	FILE* fp = fopen(filename, "rb");
	return fp;
}

FileHandle FileSystem_Posix::Create(const char* filename)
{
	FILE* fp = fopen(filename, "wb");
	return fp;
}

void FileSystem_Posix::Close(FileHandle handle)
{
	if (handle)
	{
		flose(static_cast<FILE*>(handle));
	}
}

bool FileSystem_Posix::Exist(const char* filename)
{
	return (access(filename, F_OK) != -1);
}

size_t FileSystem_Posix::Tell(FileHandle handle)
{
	return static_cast<size_t>( ftell((FILE*)handle) );
}

void FileSystem_Posix::Seek(FileHandle handle, SeekWay seekway, long offset)
{
	fseek((FILE*)handle, offset, s_stdioSeekDir[(int)seekway]);
}

size_t FileSystem_Posix::Read(FileHandle handle, void* data, size_t size)
{
	return static_cast<size_t>(fread(data, 1, size, (FILE*)handle));
}

size_t FileSystem_Posix::Write(FileHandle handle, const void* data, size_t size)
{
	return static_cast<size_t>(fwrite(data, 1, size, (FILE*)handle));
}

}

#endif // defined(__unix__) || defined(__unix) || defined(__APPLE__)
