#ifdef _WINDOWS

#include "core/platform/core_platform.h"
#include "core/file/filesystem_win32.h"

namespace solunar
{

FileSystem_Win32::FileSystem_Win32()
{
}

FileSystem_Win32::~FileSystem_Win32()
{
}

FileHandle FileSystem_Win32::open(const char* filename)
{
	DWORD dwDesiredAccess = GENERIC_READ;
	DWORD dwShareMode = FILE_SHARE_READ;

	FileHandle handle = (FileHandle)CreateFileA(filename, dwDesiredAccess, dwShareMode, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (handle == INVALID_HANDLE_VALUE)
		return nullptr;

	return handle;
}

FileHandle FileSystem_Win32::create(const char* filename)
{
	DWORD dwDesiredAccess = GENERIC_WRITE;
	DWORD dwShareMode = FILE_SHARE_WRITE;

	FileHandle handle = (FileHandle)CreateFileA(filename, dwDesiredAccess, dwShareMode, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (handle == INVALID_HANDLE_VALUE)
		return nullptr;

	return handle;
}

void FileSystem_Win32::close(FileHandle handle)
{
	if (handle)
	{
		CloseHandle(static_cast<HANDLE>(handle));
	}
}

bool FileSystem_Win32::exist(const char* filename)
{
	DWORD dwAttrib = GetFileAttributes(filename);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

size_t FileSystem_Win32::tell(FileHandle handle)
{
	return static_cast<size_t>(SetFilePointer(handle, 0, NULL, FILE_CURRENT));
}

void FileSystem_Win32::seek(FileHandle handle, SeekWay seekway, long offset)
{
	DWORD dwMoveWay = 0;

	switch (seekway)
	{
	case Seek_Begin:
		dwMoveWay = FILE_BEGIN;
		break;
	case Seek_Current:
		dwMoveWay = FILE_CURRENT;
		break;
	case Seek_End:
		dwMoveWay = FILE_END;
		break;
	}

	SetFilePointer(static_cast<HANDLE>(handle), static_cast<LONG>(offset), NULL, dwMoveWay);
}

size_t FileSystem_Win32::read(FileHandle handle, void* data, size_t size)
{
	DWORD bytesRead = 0;
	ReadFile(static_cast<HANDLE>(handle), data, static_cast<DWORD>(size), &bytesRead, NULL);
	return static_cast<size_t>(bytesRead);
}

size_t FileSystem_Win32::write(FileHandle handle, const void* data, size_t size)
{
	DWORD bytesWritten = 0;
	WriteFile(static_cast<HANDLE>(handle), data, static_cast<DWORD>(size), &bytesWritten, NULL);
	return static_cast<size_t>(bytesWritten);
}

}

#endif // _WINDOWS
