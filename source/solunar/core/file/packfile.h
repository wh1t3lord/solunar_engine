#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "core/file/datastream.h"

#include <unordered_map>
#include <string>

namespace engine
{
	struct PackHeader
	{
		char magic[9];
		size_t version;
		size_t filecount;
	};

	struct PackFileEntry
	{
		char filename[255];
		bool isCompressed;
		size_t size;
		size_t pointerOffset;
		size_t compressedSize;
	};

	class PackFile
	{
	public:
		PackFile(const std::string& filename);
		~PackFile();

		void load(const std::string& filename);

		bool hasFile(const std::string& filename);

		DataStreamPtr openFile(const std::string& filename);

		std::string getPackFilename(const std::string& filename);

	private:
		std::unordered_map<std::string, PackFileEntry> m_fileEntries;

		DataStreamPtr m_file;
		//File* m_file;
	};
}

#endif // !ARCHIVE_H
