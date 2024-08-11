#include "graphicspch.h"
#include "graphics/image.h"

#include "core/file/filesystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

namespace solunar
{
	static void stbi_write_callback(void* context, void* data, int size)
	{
		g_fileSystem->write(static_cast<FileHandle>(context), data, size);
	}

	void Image::freeImageData(void* data)
	{
		stbi_image_free(data);
	}

	Image::Image()
	{
		m_data = nullptr;
		m_width = 0;
		m_height = 0;
		m_channels = 0;
		m_miplevel = 0;
		m_flip = false;
		m_format = (ImageFormat)0;
	}

	Image::~Image()
	{
	}

	void Image::free()
	{
		if (m_data)
			Image::freeImageData(m_data);
	}

	void Image::createFromFile(const std::string& filename)
	{
		stbi_set_flip_vertically_on_load(false);

		FileHandle file = g_fileSystem->open(filename.c_str());
		g_fileSystem->seek(file, Seek_End, 0);
		size_t fileSize = g_fileSystem->tell(file);
		g_fileSystem->seek(file, Seek_Begin, 0);

		uint8_t* fileData = mem_array<uint8_t>(static_cast<size_t>(fileSize));
		Assert2(fileData, "Failed to allocate memory for texture data. Not enough space?");

		g_fileSystem->read(file, fileData, fileSize);

		m_data = stbi_load_from_memory(static_cast<stbi_uc*>(fileData), static_cast<int>(fileSize),
			&m_width, &m_height, &m_channels, STBI_rgb_alpha);

		Assert3(m_data, "stbi_load_from_memory is failed. Error:", stbi_failure_reason());
		mem_free_array(fileData);
	}

	void Image::createFromMemoryStream(const std::shared_ptr<DataStream>& stream)
	{
		stbi_set_flip_vertically_on_load(true);

		stream->seek(Seek_End, 0);
		size_t fileSize = stream->tell();
		stream->seek(Seek_Begin, 0);

		uint8_t* fileData = mem_array<uint8_t>(static_cast<size_t>(fileSize));
		Assert2(fileData, "Failed to allocate memory for texture data. Not enough space?");

		stream->read(fileData, fileSize);

		m_data = stbi_load_from_memory(static_cast<stbi_uc*>(fileData), static_cast<int>(fileSize),
			&m_width, &m_height, &m_channels, STBI_rgb_alpha);

		Assert3(m_data, "stbi_load_from_memory is failed. Error:", stbi_failure_reason());
		mem_free_array(fileData);
	}

	void Image::createDDSFromMemoryStream(const std::shared_ptr<DataStream>& stream)
	{
		Assert2(0, "DDS Loading is not implemented.");

		stream->seek(Seek_End, 0);
		size_t fileSize = stream->tell();
		stream->seek(Seek_Begin, 0);

		uint8_t* fileData = mem_array<uint8_t>(static_cast<size_t>(fileSize));
		Assert2(fileData, "Failed to allocate memory for texture data. Not enough space?");

		stream->read(fileData, fileSize);

		//m_data = stbi_load_from_memory(static_cast<stbi_uc*>(fileData), static_cast<int>(fileSize),
		//	&m_width, &m_height, &m_channels, STBI_rgb_alpha);
		//
		//Assert3(m_data, "stbi_load_from_memory is failed. Error:", stbi_failure_reason());

		mem_free_array(fileData);
	}

	//void Image::createFromFile(const std::shared_ptr<DataStream>& stream)
	//{
	//	stbi_set_flip_vertically_on_load(true);

	//	stream->seek(FileSeek::End, 0);
	//	long fileSize = stream->tell();
	//	stream->seek(FileSeek::Begin, 0);

	//	uint8_t* fileData = mem_array<uint8_t>(static_cast<size_t>(fileSize));
	//	ASSERT(fileData && "Failed to allocate memory for texture data. Not enough space?");

	//	stream->read(fileData, fileSize);

	//	m_data = stbi_load_from_memory(static_cast<stbi_uc*>(fileData), static_cast<int>(fileSize),
	//		&m_width, &m_height, &m_channels, STBI_rgb_alpha);

	//	ASSERT(m_data && "stbi_load_from_memory is failed. Something wrong! See the log file.");
	//	mem_free_array(fileData);
	//}

	void Image::createRaw(void* data, int width, int height, int channels)
	{
		m_data = data;
		m_width = width;
		m_height = height;
		m_channels = channels;
	}

	void Image::save(const std::string& filename)
	{
		FileHandle file = g_fileSystem->create(filename.c_str());

		stbi_flip_vertically_on_write(m_flip);

		size_t extPos = filename.find('.');
		std::string ext = filename.substr(extPos + 1);

		if (ext == "png")
			stbi_write_png_to_func(stbi_write_callback, (void*)file, m_width, m_height, m_channels, m_data, m_width * m_channels);
		else if (ext == "tga")
			stbi_write_tga_to_func(stbi_write_callback, (void*)file, m_width, m_height, m_channels, m_data);
		else if (ext == "jpg")
			stbi_write_jpg_to_func(stbi_write_callback, (void*)file, m_width, m_height, m_channels, m_data, 90);
		else
			throw std::logic_error("Image::save: unknowed format to save!");

		g_fileSystem->close(file);
	}

	//void Image::save(const std::shared_ptr<DataStream>& stream)
	//{
	//	stbi_flip_vertically_on_write(m_flip);
	//	stbi_write_png_to_func(stbi_write_callback, (void*)stream.get(), m_width, m_height, m_channels, m_data, m_width * m_channels);
	//}

	int Image::getWidth()
	{
		return m_width;
	}

	int Image::getHeight()
	{
		return m_height;
	}

	int Image::getChannels()
	{
		return m_channels;
	}

	int Image::getmiplevel()
	{
		return m_miplevel;
	}

	ImageFormat Image::getImageFormat()
	{
		return m_format;
	}

	void* Image::getData()
	{
		return m_data;
	}

	void Image::resize(int width, int height)
	{

	}
}