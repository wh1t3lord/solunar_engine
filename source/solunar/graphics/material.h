#ifndef MATERIAL_H
#define MATERIAL_H

#include "graphics/shaderprogram.h"
#include "graphics/ShaderProgramManager.h"
#include "graphics/texturemap.h"
#include "graphics/graphicsobject.h"

namespace solunar
{
	enum MAT_TEX
	{
		MAT_TEX_DIFFUSE,
		MAT_TEX_NORMALMAP,
		MAT_TEX_DETAIL,
	};

	class TextureMap;
	class IMaterialInstance;
	class IShaderProgram;
	struct IRasterizerState;
	struct IBlendState;

	class Material;
	
	IShaderProgram* createShaderProgramFromMaterial(Material* material);
	
	enum class MaterialType
	{
		Null,
		Generic,
		Masked
	};

	class Material : public GraphicsObject
	{
		friend IShaderProgram* createShaderProgramFromMaterial(Material* material);
		
		DECLARE_OBJECT(Material);
	public:
		static void createMaterialFromImport(const char* name, const char* diffuseName, const char* normalName);

	public:
		Material();
		~Material();

		void Load(const std::shared_ptr<DataStream>& dataStream) override;

		void LoadXML(tinyxml2::XMLElement& element) override;
		void SaveXML(tinyxml2::XMLElement& element) override;

	public:
		void setName(const std::string& name) { m_materialName = name; }

		void CreateHw() override;
		void ReleaseHw() override;

		void bindSamplers();

		void bindUniformsCustom(IShaderProgram* shader);

		void bind();

		std::string& GetName() { return m_materialName; }

		std::weak_ptr<TextureMap> GetTexture(MAT_TEX tex);
		MaterialType getMaterialType() { return m_materialType; }

		IMaterialInstance* GetMaterialInstance() { return m_materialInstance; }

		// #TODO: VERY BAD, LIKE GETTERS
	public:
		IMaterialInstance* m_materialInstance;

		ISamplerState* m_albedoSampler;

		std::weak_ptr<TextureMap> m_albedoTexture;
		std::weak_ptr<TextureMap> m_normalTexture;
		std::weak_ptr<TextureMap> m_specularTexture;

		std::string m_materialName;
		std::string m_shaderName;
		
		std::string m_albedoTextureFileName;
		std::string m_normalTextureName;
		std::string m_specularTextureName;

		glm::vec3 m_albedoColor;

		MaterialType m_materialType;

		bool m_depthWrite;
		bool m_skipmips;
		bool m_selfillum;
		bool m_clampToEdge;
		bool m_pointFilter;
		bool m_isFirstPersonWeapon;

		IRasterizerState* m_rasterizerDesc;
	};

	class NullMaterial : public Material
	{
		DECLARE_OBJECT(NullMaterial);
	public:
		NullMaterial();
		~NullMaterial();

		static void RegisterObject();
	};

	class GenericMaterial : public Material
	{
		DECLARE_OBJECT(GenericMaterial);
	public:
		GenericMaterial();
		~GenericMaterial();

		static void RegisterObject();
	};

	class LayeredMaskedMaterial : public Material
	{
		DECLARE_OBJECT(LayeredMaskedMaterial);
	public:
		LayeredMaskedMaterial();
		~LayeredMaskedMaterial();

		static void RegisterObject();
	};
}

#endif // !MATERIAL_H
