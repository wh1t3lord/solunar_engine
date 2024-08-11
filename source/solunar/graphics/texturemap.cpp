#include "graphicspch.h"
#include "graphics/core/device.h"
#include "graphics/image.h"
#include "graphics/texturemap.h"
#include "graphics/graphicsoptions.h"


namespace solunar
{
	TextureMap::TextureMap() :
		m_bManualCreated(false)
	{
		memset(&m_textureDesc, 0, sizeof(m_textureDesc));
		memset(&m_textureSubresourceDesc, 0, sizeof(m_textureSubresourceDesc));
	}

	TextureMap::~TextureMap()
	{
		Release();
	}

	std::shared_ptr<TextureMap> TextureMap::create2DFromSource(ITexture2D* pTexture2D, const TextureDesc& textureDesc)
	{
		// create object
		std::shared_ptr<TextureMap> pTextureMap;//= g_typeManager->CreateObject<TextureMap>();

		// Fill private fields
		pTextureMap->m_texture2D = pTexture2D;
		pTextureMap->m_textureDesc = textureDesc;
		pTextureMap->m_bManualCreated = true;

		// #TODO: Sampler 
		pTextureMap->setMin(TextureFilter::Linear);
		pTextureMap->setMag(TextureFilter::Linear);
		pTextureMap->setWrapS(TextureWrap::Repeat);
		pTextureMap->setWrapT(TextureWrap::Repeat);
	
		return pTextureMap;
	}

	void TextureMap::Release()
	{
		if (m_texture2D) {
			mem_delete(m_texture2D);
			m_texture2D = nullptr;
		}

		if (m_textureSubresourceDesc.m_memory)
			Image::freeImageData(m_textureSubresourceDesc.m_memory);
	}

	void TextureMap::bind()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void TextureMap::createHw()
	{
		m_texture2D = g_renderDevice->CreateTexture2D(m_textureDesc, m_textureSubresourceDesc);
		Image::freeImageData(m_textureSubresourceDesc.m_memory);
		m_textureSubresourceDesc.m_memory = nullptr;
	}

	void TextureMap::Load(const std::shared_ptr<DataStream>& dataStream)
	{
		Image image;
		image.setFlip(true);
		// image.createFromFile(dataStream);

		// Load header of file
		unsigned long dwMagic = 0;
		dataStream->Read(&dwMagic);

		// reset stream
		dataStream->Seek(Seek_Begin, 0);

		// is not DDS
		if (dwMagic != 0x20534444) {
			image.createFromMemoryStream(dataStream);
		// is DDS
		} else {
			image.createDDSFromMemoryStream(dataStream);
		}

		m_textureDesc.m_textureType = TextureType::Texture2D;
		m_textureDesc.m_width = image.getWidth();
		m_textureDesc.m_height = image.getHeight();
		m_textureDesc.m_format = ImageFormat::RGBA32;
		m_textureSubresourceDesc.m_memory = image.getData();
		m_textureSubresourceDesc.m_memoryPitch = image.getWidth() * 4;

		createHw();
	}

	void TextureMap::setWrapS(TextureWrap wrap)
	{
		//m_texture2D->setWrapS(wrap);
	}

	void TextureMap::setWrapT(TextureWrap wrap)
	{
		//m_texture2D->setWrapT(wrap);
	}

	void TextureMap::setMin(TextureFilter filter)
	{
		//m_texture2D->setMin(filter);
	}

	void TextureMap::setMag(TextureFilter filter)
	{
		//m_texture2D->setMag(filter);
	}

	void TextureMap::setAnisotropicLevel(int level)
	{
		
	}

	void TextureMap::gen_mipmaps()
	{
		//if (!m_textureDesc.m_isCompressed)
		//{
		//	if (!m_textureDesc.m_mipmapping)
		//		m_textureDesc.m_mipmapping = true;

		//	glGenerateMipmap(GL_TEXTURE_2D);
		//}
	}

	void TextureMap::destroyHW()
	{
	}

	TextureMapCube::TextureMapCube()
	{
	}

	TextureMapCube::~TextureMapCube()
	{
	}

	void TextureMapCube::RegisterObject()
	{
		g_typeManager->RegisterObject<TextureMapCube>();
	}

	void TextureMapCube::Load(const std::shared_ptr<DataStream>& dataStream)
	{
		//std::shared_ptr<XMLDoc> xml(new XMLDoc(dataStream));

		//// reset stream
		//dataStream->Seek(FileSeek::Begin, 0);

		//// read each face
		//tinyxml2::XMLElement* rootElement = xml->getDocument().FirstChildElement();
		//tinyxml2::XMLElement* faceElement = rootElement->FirstChildElement("face");

		//while (faceElement)
		//{

		//	faceElement = faceElement->NextSiblingElement("face");
		//}
	}

	void TextureMapCube::loadFace(int face, std::shared_ptr<Image> image)
	{
	}
}
