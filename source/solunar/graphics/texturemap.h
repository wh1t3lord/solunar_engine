#ifndef TEXTUREMAP_H
#define TEXTUREMAP_H

#include "graphics/core/texture.h"
#include "graphics/image.h"
#include "graphics/graphicsobject.h"

namespace solunar
{
	class TextureMap : public GraphicsObject
	{
	public:
		DECLARE_OBJECT(TextureMap);

	public:
		static std::shared_ptr<TextureMap> create2DFromSource(ITexture2D* pTexture2D, const TextureDesc& textureDesc);

	public:
		TextureMap();
		virtual ~TextureMap();

		virtual void Load(const std::shared_ptr<DataStream>& dataStream) override;
	
		void Release();
		ITexture2D* getHWTexture() { return m_texture2D; }

		void bind();

		void CreateHw();
		void destroyHW();

		void setWrapS(TextureWrap wrap);
		void setWrapT(TextureWrap wrap);
		void setMin(TextureFilter filter);
		void setMag(TextureFilter filter);

		void setAnisotropicLevel(int level);

		void gen_mipmaps();

		bool isManualCreated() { return m_bManualCreated; }

	private:
		ITexture2D* m_texture2D;
		TextureDesc m_textureDesc;
		SubresourceDesc m_textureSubresourceDesc;
		bool m_bManualCreated;
	};


	// Cubemap
	class TextureMapCube : public TextureMap
	{
		DECLARE_OBJECT(TextureMapCube);
	public:
		TextureMapCube();
		~TextureMapCube();

		static void RegisterObject();

		void Load(const std::shared_ptr<DataStream>& dataStream) override;

		void loadFace(int face, std::shared_ptr<Image> image);
	};

}

#endif // TEXTUREMAP_H
