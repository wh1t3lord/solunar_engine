#include "core/core.h"
#include "core/debug.h"
#include <memory>
#include <string>
#include "core/file/datastream.h"
#include "core/file/contentdevice.h"
#include "core/file/filesystem.h"
#include "core/file/filestream.h"
#include "core/file/filesystem_pack.h"

namespace engine
{

ContentDevice* g_contentDevice = nullptr;

ContentDevice::ContentDevice(const std::string& path) :
	m_path(path)
{
}

ContentDevice::~ContentDevice()
{
}

DataStreamPtr ContentDevice::openStream(const std::string& filename)
{
	Assert2(g_fileSystem, "Content device called before file system initialization.");

	// compute path
	std::string path = m_path;
	path += "/";
	path += filename;

	if (g_fileSystemPack.exist(filename))
		return g_fileSystemPack.openStream(filename, true);

	if (!g_fileSystem->exist(path.c_str()))
		return DataStreamPtr(nullptr);

	// create object
	return std::make_shared<FileStream>(path);
}


}