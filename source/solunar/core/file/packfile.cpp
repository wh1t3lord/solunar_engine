#include "core/core.h"
#include "core/platform/core_platform.h"
#include "core/debug.h"
#include "core/file/packfile.h"
#include "core/file/filestream.h"

#include <lz4.h>

namespace solunar
{
	class PackFileStream : public DataStream
	{
	public:
		PackFileStream(DataStreamPtr pkgFile, const PackFileEntry& fileEntry) :
			m_pkgFile(pkgFile),
			m_fileEntry(fileEntry),
			m_currentPosition(0)
		{
			if (fileEntry.isCompressed)
			{
				m_pkgFile->seek(Seek_Begin, fileEntry.pointerOffset);

				std::vector<char> rawData;
				rawData.resize(fileEntry.compressedSize);
				m_pkgFile->read(rawData.data(), rawData.size());

				m_fileData.resize(fileEntry.size);

				LZ4_decompress_safe(rawData.data(), m_fileData.data(), fileEntry.compressedSize, fileEntry.size);
			}
			else
			{
				m_fileData.resize(fileEntry.size);
				m_pkgFile->seek(Seek_Begin, fileEntry.pointerOffset);
				m_pkgFile->read(m_fileData.data(), m_fileData.size());
			}
		}

		size_t read(void* buffer, size_t count) override
		{
			memcpy(buffer, m_fileData.data() + m_currentPosition, count);
			return count;
		}

		size_t write(void* buffer, size_t count) override
		{
			Core::error("The method or operation is not implemented.");
			return count;
		}

		void seek(SeekWay dir, long offset) override
		{
			long positionChange = 0;

			switch (dir)
			{
			case Seek_Begin:
				positionChange = offset;
				break;
			case Seek_Current:
				positionChange = m_currentPosition + offset;
				break;
			case Seek_End:
				positionChange = m_fileEntry.size + offset;
				break;
			default:
				break;
			}

			m_currentPosition = positionChange;
		}

		long tell() override
		{
			return m_currentPosition;
		}

		bool eof() override
		{
			return m_currentPosition >= m_fileEntry.size;
		}

		void flush() override
		{
			Core::error("The method or operation is not implemented.");
		}

	private:
		std::vector<char> m_fileData;
		const PackFileEntry& m_fileEntry;
		DataStreamPtr m_pkgFile;
		long m_currentPosition;
	};

	PackFile::PackFile(const std::string& filename)
	{
		m_file = nullptr;
		load(filename);
	}

	PackFile::~PackFile()
	{
		m_file.reset();
		m_file = nullptr;
	}

	void PackFile::load(const std::string& filename)
	{
		//m_file = allocatorNew<File>(*g_sysAllocator, filename, FileAccess::Read);

		m_file = std::make_shared<FileStream>(filename); //std::static_pointer_cast<DataStream>(std::make_shared<FileMemoryMappingStream>(filename));

		PackHeader header;
		m_file->read(&header, sizeof(header));

		if (strcmp(header.magic, "packfile") != 0)
			Core::error("PackFile::load: %s is not a packfile or file has been damaged.", filename.c_str());

		if (header.version != 1)
			Core::error("PackFile::load: packfile mismatch version.");

		std::vector<PackFileEntry> fileEntries;
		fileEntries.resize(header.filecount);
		m_file->read(fileEntries.data(), sizeof(PackFileEntry) * fileEntries.size());

		for (auto it : fileEntries)
			m_fileEntries.emplace(it.filename, it);
	}

	bool PackFile::hasFile(const std::string& filename)
	{
		std::string packFilename = getPackFilename(filename);

		auto f = m_fileEntries.find(packFilename);
		if (f != m_fileEntries.end())
			return true;

		return false;
	}

	DataStreamPtr PackFile::openFile(const std::string& filename)
	{
		std::string packFilename = getPackFilename(filename);

		auto f = m_fileEntries.find(packFilename);
		if (f != m_fileEntries.end())
		{
			return std::make_shared<PackFileStream>(m_file, (*f).second);
		}

		return std::shared_ptr<DataStream>();
	}

	std::string PackFile::getPackFilename(const std::string& filename)
	{
		if (filename.find("data/") != std::string::npos)
		{
			size_t dataLocation = filename.find_first_of("data/");

			std::string newFilename = filename.substr(strlen("data/"), filename.length());
			return newFilename;
		}

		return filename;
	}

}
