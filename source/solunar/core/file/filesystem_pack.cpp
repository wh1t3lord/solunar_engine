#include "core/core.h"
#include "core/platform/core_platform.h"
#include "core/debug.h"
#include "core/memory/memorymanager.h"
#include "core/file/filesystem_pack.h"

namespace solunar
{

FileSystemPack g_fileSystemPack;

FileSystemPack::FileSystemPack()
{
}

FileSystemPack::~FileSystemPack()
{
}

void FileSystemPack::initialize()
{
#ifdef WIN32
	const char* findToFind = "*.packfile";
	WIN32_FIND_DATAA ffd;
	HANDLE hFind;

	hFind = FindFirstFileA(findToFind, &ffd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (ffd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{
				Core::Msg("[filesystem]: loading %s", ffd.cFileName);
				m_packFiles.push_back(mem_new<PackFile>(ffd.cFileName));
			}
		} while (FindNextFileA(hFind, &ffd) != 0);

		FindClose(hFind);
	}
#endif
}

void FileSystemPack::shutdown()
{
	for (auto it : m_packFiles)
	{
		if (it)
		{
			mem_delete(it);
			it = nullptr;
		}
	}

	m_packFiles.clear();
}

DataStreamPtr FileSystemPack::openStream(const std::string& filename, bool isReading)
{
	for (auto it : m_packFiles)
	{
		if (it && it->hasFile(filename))
			return it->openFile(filename);
	}
	
	return DataStreamPtr(); //m_fileSystem->openStream(filename, isReading);
}

DataStreamPtr FileSystemPack::openAsyncStream(const std::string& filename, bool isReading)
{
	Assert(0);
	return DataStreamPtr();
}

bool FileSystemPack::exist(const std::string& filename)
{
	for (auto it : m_packFiles)
		if (it && it->hasFile(filename))
			return true;

	return false; //m_fileSystem->exist(filename);
}

}