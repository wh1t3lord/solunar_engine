#include "core/core.h"
#include "core/debug.h"
#include <memory>
#include <string>
#include "core/file/datastream.h"
#include "core/file/contentdevice.h"
#include "core/file/filesystem.h"
#include "core/file/filestream.h"
#include "core/file/filesystem_pack.h"

namespace solunar
{

ContentDevice* g_contentDevice = nullptr;

ContentDevice::ContentDevice(const std::string& path) :
	m_path(path)
{
}

ContentDevice::~ContentDevice()
{
}

DataStreamPtr ContentDevice::OpenStream(const std::string& filename)
{
	Assert2(g_fileSystem, "Content device called before file system initialization.");

	// compute path
	std::string path = m_path;
	path += "/";
	path += filename;

	// open an stream from packfile
	if (g_fileSystemPack.exist(filename))
		return g_fileSystemPack.openStream(filename, true);

	// open an stream from absolute path
	if (g_fileSystem->Exist(filename.c_str()))
		return std::make_shared<FileStream>(filename);

	if (!g_fileSystem->Exist(path.c_str()))
		return DataStreamPtr(nullptr);

	// create object
	return std::make_shared<FileStream>(path);
}


}