#ifndef FILESYSTEMPACK_H
#define FILESYSTEMPACK_H

#include <memory>
#include <string>
#include <vector>

#include "core/file/datastream.h"
#include "core/file/filesystem.h"
#include "core/file/packfile.h"

namespace engine
{

class FileSystemPack 
{
public:
	FileSystemPack();
	~FileSystemPack();

	void initialize();
	void shutdown();

	DataStreamPtr openStream(const std::string& filename, bool isReading);
	DataStreamPtr openAsyncStream(const std::string& filename, bool isReading);

	bool exist(const std::string& filename);

private:
	std::vector<PackFile*> m_packFiles;
};

extern FileSystemPack g_fileSystemPack;

}

#endif // !FILESYSTEMPACK_H
