#include "graphicspch.h"
#if 0
#include "graphics/ui/rmlfilesystem.h"

#include "core/file/datastream.h"
#include "core/file/filesystem.h"

#include "engine/content/contentmanager.h"

namespace engine
{

struct RmlFileHandle
{
	std::shared_ptr<DataStream> m_dataStream;
};

RmlFileSystem* g_rmlFileSystem = nullptr;

Rml::FileHandle RmlFileSystem::Open(const Rml::String& path)
{
	RmlFileHandle* fileHandle = mem_new<RmlFileHandle>();
	fileHandle->m_dataStream = g_contentManager->openStream(path);
	return (Rml::FileHandle)fileHandle;
}

void RmlFileSystem::Close(Rml::FileHandle file)
{
	RmlFileHandle* fileHandle = (RmlFileHandle*)file;
	mem_delete(fileHandle);
}

size_t RmlFileSystem::Read(void* buffer, size_t size, Rml::FileHandle file)
{
	RmlFileHandle* fileHandle = (RmlFileHandle*)file;
	ASSERT(fileHandle->m_dataStream);

	long startRead = fileHandle->m_dataStream->tell();
	fileHandle->m_dataStream->read(buffer, size);
	long endRead = fileHandle->m_dataStream->tell();

	return endRead - startRead;
}

bool RmlFileSystem::Seek(Rml::FileHandle file, long offset, int origin)
{
	RmlFileHandle* fileHandle = (RmlFileHandle*)file;
	ASSERT(fileHandle->m_dataStream);

	FileSeek seekDir;
	if (origin == SEEK_SET)
		seekDir = FileSeek::Begin;
	else if (origin == SEEK_CUR)
		seekDir = FileSeek::Current;
	else if (origin == SEEK_END)
		seekDir = FileSeek::End;
	else
		Core::error("RmlFileSystem::Seek: unknowed origin value.");

	fileHandle->m_dataStream->seek(seekDir, offset);

	return true;
}

size_t RmlFileSystem::Tell(Rml::FileHandle file)
{
	RmlFileHandle* fileHandle = (RmlFileHandle*)file;
	ASSERT(fileHandle->m_dataStream);

	return (size_t)fileHandle->m_dataStream->tell();
}

//size_t RmlFileSystem::Length(Rml::FileHandle file)
//{
//	return size_t();
//}

//bool RmlFileSystem::LoadFile(const Rml::String & path, Rml::String & out_data)
//{
//	return false;
//}

}
#endif
