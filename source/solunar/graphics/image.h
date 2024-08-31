#ifndef IMAGE_H
#define IMAGE_H

#include "graphics/gfxcommon.h"

namespace solunar
{
	// wrapper on image loading and other stuff
	class Image
	{

	public:
		static void freeImageData(void* data);

	public:
		Image();
		~Image();

		void free();

		void createFromFile(const std::string& filename);
		void createFromMemoryStream(const std::shared_ptr<DataStream>& stream);
		void createDDSFromMemoryStream(const std::shared_ptr<DataStream>& stream);

		// create from existed data
		void createRaw(void* data, int width, int height, int channels);

		// saving
		void Save(const std::string& filename);
		//void Save(const std::shared_ptr<DataStream>& stream);

		int getWidth();
		int getHeight();
		int getChannels();
		int getmiplevel();
		ImageFormat getImageFormat();

		void* getData();

		void setFlip(bool flip) { m_flip = flip; }
		bool getFlip() { return m_flip; }

		// image operations
		void resize(int width, int height);

	private:
		void* m_data;
		int m_width, m_height, m_channels, m_miplevel;
		ImageFormat m_format;

		bool m_flip;
	};
}

#endif // !IMAGE_H
