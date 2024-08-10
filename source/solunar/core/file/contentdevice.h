#ifndef FILE_CONTENTDEVICE_H
#define FILE_CONTENTDEVICE_H

#include <string>

#include "core/utils/singleton.h"
#include "core/file/datastream.h"

namespace solunar
{

class ContentDevice
{
public:
	ContentDevice(const std::string& path);
	~ContentDevice();

	DataStreamPtr openStream(const std::string& filename);

	const std::string& getPath() { return m_path; }

private:
	std::string m_path;
};

extern ContentDevice* g_contentDevice;

}

#endif // !FILE_CONTENTDEVICE_H
