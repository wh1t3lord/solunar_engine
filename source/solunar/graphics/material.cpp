#include "graphicspch.h"

#include "core/file/filesystem.h"

#include "graphics/material.h"
#include "graphics/texturemap.h"
#include "graphics/shaderprogram.h"
#include "graphics/ShaderProgramManager.h"
#include "graphics/rendercontext.h"
#include "graphics/shaderconstantmanager.h"
#include "graphics/graphicsoptions.h"

#include "graphics/core/device.h"
#include "graphics/core/statemanager.h"

#include "graphics/materials/materialinstance.h"
#include "graphics/materials/materialinstancefactory.h"

#include "engine/camera.h"

#include "engine/camera.h"
#include "engine/entity/world.h"
#include "graphics/graphicsworld.h"

#include <sstream>

namespace engine
{
	MaterialType getMaterialTypeFromName(const std::string& shaderName)
	{
		if (shaderName == "diffuse")
		{
			return MaterialType::Generic;
		}
		else if (shaderName == "generic" || shaderName == "Generic")
		{
			return MaterialType::Generic;
		}
		else
		{
			Core::error("getMaterialType: Doesnt support old shader %s", shaderName);	
		}

		return MaterialType::Null;
	}
	
	void Material::createMaterialFromImport(const char* name, const char* diffuseName, const char* normalName)
	{
		std::string materiaName;
		materiaName += "data/";
		materiaName += "materials/";
		materiaName += name;
		materiaName += ".xml";

		//File* file = FileDevice::getInstance()->openFile(materiaName, FileAccess::Write);
		FileHandle file = g_fileSystem->create(materiaName.c_str());

		tinyxml2::XMLDocument doc;
		tinyxml2::XMLElement* rootElement = doc.NewElement("Material");

		Material* materialToWrite = TypeManager::getInstance()->createObject<Material>();
		materialToWrite->m_albedoTextureName = diffuseName;
		materialToWrite->m_normalTextureName = normalName;
		
		materialToWrite->saveXML(*rootElement);;

		doc.InsertFirstChild(rootElement);

		tinyxml2::XMLPrinter visitor;
		doc.Accept(&visitor);

		std::string text = visitor.CStr();
		text += '\0';

		g_fileSystem->write(file, (void*)text.data(), text.size());

		mem_delete(materialToWrite);
		materialToWrite = nullptr;

		g_fileSystem->close(file);
	}

	Material::Material()
	{
		m_albedoColor = glm::vec3(1.0f);
		m_albedoSampler = nullptr;
		m_depthWrite = true;
		m_selfillum = false;
		m_clampToEdge = false;
		m_skipmips = false;
		m_materialInstance = nullptr;
		m_isFirstPersonWeapon = false;
	}

	Material::~Material()
	{
		if (m_materialInstance)
		{
			mem_delete(m_materialInstance);
			m_materialInstance = nullptr;
		}

		//m_shader = nullptr;

		m_specularTexture.reset();
		m_normalTexture.reset();
		m_albedoTexture.reset();
	
		m_albedoSampler = nullptr;
	}

	void Material::load(const std::shared_ptr<DataStream>& dataStream)
	{
		dataStream->seek(Seek_End, 0);
		size_t length = dataStream->tell();
		dataStream->seek(Seek_Begin, 0);

		char* data = new char[length + 1];
		dataStream->read((void*)data, length);
		data[length] = '\0';

		//parse(data, length);

		tinyxml2::XMLDocument doc;
		doc.Parse(data, length);

		tinyxml2::XMLElement* materialElement = doc.FirstChildElement("Material");
		loadXML(*materialElement);

		delete[] data;
	}

	void Material::loadXML(tinyxml2::XMLElement& element)
	{
		if (strcmp(element.Name(), "Material") != 0) {
			Core::error("Material::loadXML: failed to load material. XMLElement is not a \"Material\".");
		}

		tinyxml2::XMLElement* shaderElement = element.FirstChildElement("Shader");
		if (shaderElement) {
			m_materialType = getMaterialTypeFromName(shaderElement->GetText());
			
		} else {
			Core::error("Material::loadXML: failed to load material. Doesnt exist \"Shader\" element.");
		}

		// Albedo texture
		tinyxml2::XMLElement* albedoTexture = element.FirstChildElement("AlbedoTexture");
		if (albedoTexture) {
			const tinyxml2::XMLAttribute* filename = albedoTexture->FindAttribute("Filename");
			if (!filename)
				filename = albedoTexture->FindAttribute("filename");

			if (filename) {
				m_albedoTextureName = filename->Value();
			} else {
				m_albedoTextureName = "textures/notexture.png";
			}

			const tinyxml2::XMLAttribute* clampToEdge = albedoTexture->FindAttribute("ClampToEdge");
			if (clampToEdge) {
				m_clampToEdge = clampToEdge->BoolValue();
			}
		} else {
			m_albedoTextureName = "textures/notexture.png";
		}

		// Normal texture
		tinyxml2::XMLElement* normalTexture = element.FirstChildElement("NormalTexture");
		if (normalTexture) {
			const tinyxml2::XMLAttribute* filename = normalTexture->FindAttribute("Filename");
			if (!filename)
				filename = normalTexture->FindAttribute("filename");

			if (filename) {
				m_normalTextureName = filename->Value();
			}
		} else {
			m_normalTextureName = "";
		}

		// Specular texture
		tinyxml2::XMLElement* specularTexture = element.FirstChildElement("SpecularTexture");
		if (specularTexture) {
			const tinyxml2::XMLAttribute* filename = specularTexture->FindAttribute("Filename");
			if (!filename)
				filename = specularTexture->FindAttribute("filename");

			if (filename) {
				m_specularTextureName = filename->Value();
			}
		} else {
			m_specularTextureName = "";
		}

		// albedo color
		tinyxml2::XMLElement* albedoColor = element.FirstChildElement("AlbedoColor");
		if (albedoColor) {

			const tinyxml2::XMLAttribute* r = albedoColor->FindAttribute("r");
			const tinyxml2::XMLAttribute* g = albedoColor->FindAttribute("g");
			const tinyxml2::XMLAttribute* b = albedoColor->FindAttribute("b");

			m_albedoColor.r = r->FloatValue();
			m_albedoColor.g = g->FloatValue();
			m_albedoColor.b = b->FloatValue();
		}

		// Render state
		tinyxml2::XMLElement* renderState = element.FirstChildElement("RenderState");
		if (renderState) {

			tinyxml2::XMLElement* depthWrite = renderState->FirstChildElement("DepthWrite");
			if (depthWrite) {
				const tinyxml2::XMLAttribute* depthWriteAttribute = depthWrite->FindAttribute("value");
				if (strcmp(depthWriteAttribute->Value(), "true") == 0) {
					m_depthWrite = true;
				} else if (strcmp(depthWriteAttribute->Value(), "false") == 0) {
					m_depthWrite = false;
				} else {
					Core::error("Material::loadXML: failed to load material. Wrong value in the \"DepthWrite\" element. Possible only true/false.");
				}
			}

			tinyxml2::XMLElement* selfillum = renderState->FirstChildElement("SelfIllumination");
			if (selfillum) {
				const tinyxml2::XMLAttribute* selfillumAttribute = selfillum->FindAttribute("value");
				if (strcmp(selfillumAttribute->Value(), "true") == 0) {
					m_selfillum = true;
				}
				else if (strcmp(selfillumAttribute->Value(), "false") == 0) {
					m_selfillum = false;
				}
				else {
					Core::error("Material::loadXML: failed to load material. Wrong value in the \"SelfIllumination\" element. Possible only true/false.");
				}
			}
		} else {
			Core::error("Material::loadXML: failed to load material. Doesnt exist \"RenderState\" element.");
		}

		createHw();
	}

	void Material::saveXML(tinyxml2::XMLElement& element)
	{
		// Shader
		tinyxml2::XMLElement* shaderElement = element.InsertNewChildElement("Shader"); //doc.NewElement("Shader");
		shaderElement->SetText("diffuse");

		// Texture's

		// Albedo
		tinyxml2::XMLElement* albedoTexture = element.InsertNewChildElement("AlbedoTexture");
		albedoTexture->SetAttribute("filename", m_albedoTextureName.c_str());
		if (m_clampToEdge) {
			albedoTexture->SetAttribute("ClampToEdge", m_clampToEdge);
		}
		else if (m_skipmips) {
			albedoTexture->SetAttribute("SkipMips", m_skipmips);
		}

		// Normal map
		tinyxml2::XMLElement* normalTexture = element.InsertNewChildElement("NormalTexture");
		normalTexture->SetAttribute("filename", m_normalTextureName.c_str());
		
		// Specular map
		tinyxml2::XMLElement* specularTexture = element.InsertNewChildElement("SpecularTexture");
		specularTexture->SetAttribute("filename", m_specularTextureName.c_str());

		// Render states
		tinyxml2::XMLElement* renderStates = element.InsertNewChildElement("RenderState");

		tinyxml2::XMLElement* depthWrite = renderStates->InsertNewChildElement("DepthWrite");
		depthWrite->SetAttribute("value", m_depthWrite);

		tinyxml2::XMLElement* selfillum = renderStates->InsertNewChildElement("SelfIllumination");
		selfillum->SetAttribute("value", m_selfillum);
	}

	std::weak_ptr<TextureMap> loadMaterialTexture(const std::string& path)
	{
		/*if (!g_fileSystem->exist(path))
		{
			Core::msg("[content]: not found %s, using notexture ...", path.c_str());
			return ContentManager::getInstance()->loadTexture("textures/notexture.png");
		}

		return ContentManager::getInstance()->loadTexture(path);*/

		return dynamicCastPtr< TextureMap >(g_contentManager->load("textures/notexture.png", TextureMap::getStaticTypeInfo()));
	}

	void Material::bindSamplers()
	{
		// OPTICK_EVENT("Material::bindSamplers");
		if (std::shared_ptr<TextureMap> albedoTexture = m_albedoTexture.lock())
		{
			// bind texture slot 0 sampler
			g_renderDevice->setSampler(0, m_albedoSampler);

			// activate diffuse texture as 0
			g_renderDevice->setTexture2D(0, albedoTexture->getHWTexture());

		}

		// Set up normal texture as 1
		if (std::shared_ptr<TextureMap> normalTexture = m_normalTexture.lock())
		{
			// #TODO: HACK
			g_renderDevice->setSampler(1, m_albedoSampler);

			g_renderDevice->setTexture2D(1, normalTexture->getHWTexture());
		}

		// Set up specular texture as 2
		if (std::shared_ptr<TextureMap> specularTexture = m_specularTexture.lock())
		{
			// #TODO: HACK
			g_renderDevice->setSampler(2, m_albedoSampler);

			g_renderDevice->setTexture2D(2, specularTexture->getHWTexture());
		}
	}

	void Material::bindUniformsCustom(IShaderProgram* shader)
	{
		
	}

	void Material::bind()
	{
		// OPTICK_EVENT("Material::bind");
		bindSamplers();
	}

	std::weak_ptr<TextureMap> Material::getTexture(MAT_TEX tex)
	{
		switch (tex)
		{
		case MAT_TEX_DIFFUSE: return m_albedoTexture;
		case MAT_TEX_NORMALMAP: return m_normalTexture;
		}

		return std::weak_ptr<TextureMap>();
	}

	void Material::createHw()
	{
		//initializeShader();

		m_materialInstance = MaterialInstanceFactory::getInstance()->createMaterialInstance(this);

		SamplerDesc albedoSamplerDesc;
		memset(&albedoSamplerDesc, 0, sizeof(albedoSamplerDesc));

		// if clamp to edge is enable
		if (m_clampToEdge)
		{
			albedoSamplerDesc.m_wrapS = TextureWrap::ClampToEdge;
			albedoSamplerDesc.m_wrapT = TextureWrap::ClampToEdge;
		}
		else
		{
			albedoSamplerDesc.m_wrapS = TextureWrap::Repeat;
			albedoSamplerDesc.m_wrapT = TextureWrap::Repeat;
		}

		m_skipmips = true;

		// if we skip the texture mip maps
		if (m_skipmips)
		{
			albedoSamplerDesc.m_minFilter = TextureFilter::Nearest;
			albedoSamplerDesc.m_magFilter = TextureFilter::Linear;
		}
		else
		{
			albedoSamplerDesc.m_minFilter = TextureFilter::LinearMipmapLinear;
			albedoSamplerDesc.m_magFilter = TextureFilter::Linear;
		}

		// set anisotropy filter based on what we have in graphics options

		int anisotropicQuality = g_graphicsOptions.m_anisotropicQuality;
		if (anisotropicQuality <= 0)
			anisotropicQuality = 1;

		albedoSamplerDesc.m_anisotropyLevel = (float)anisotropicQuality;

		m_albedoSampler = g_stateManager->createSamplerState(albedoSamplerDesc); //g_renderDevice->createSamplerState(albedoSamplerDesc);

		//ContentManager* contentManager = ContentManager::getInstance();

		//if (!g_fileSystem->exist(m_albedoTextureName))
		//{
		//	Core::msg("[content]: not found %s, using notexture ...", m_albedoTextureName.c_str());
		//	m_albedoTexture = contentManager->loadTexture("textures/notexture.png");
		//}
		//else
		//{
		//	m_albedoTexture = contentManager->loadTexture(m_albedoTextureName);
		//}

		m_albedoTexture = dynamicCastPtr< TextureMap >(g_contentManager->load(m_albedoTextureName, TextureMap::getStaticTypeInfo()));

		// REWRITE TO SAMPLER !!!

#if 0
		// if we dont skip mip mapping in the albedo texture when generate them
		if (!m_skipmips)
		{
			GLTexture2D* texture2DImpl = reinterpret_cast<GLTexture2D*>(m_albedoTexture->getHWTexture());
			glGenerateTextureMipmapEXT(texture2DImpl->getHandle(), GL_TEXTURE_2D);
		}

		if (!m_normalTextureName.empty())
			m_normalTexture = ContentManager::getInstance()->loadTexture(m_normalTextureName);

		// if we dont skip mip mapping in the albedo texture when generate them
		if (!m_skipmips && m_normalTexture)
		{
			GLTexture2D* texture2DImpl = reinterpret_cast<GLTexture2D*>(m_normalTexture->getHWTexture());
			glGenerateTextureMipmapEXT(texture2DImpl->getHandle(), GL_TEXTURE_2D);
		}

		if (!m_specularTextureName.empty())
			m_specularTexture = ContentManager::getInstance()->loadTexture(m_specularTextureName);

		// if we dont skip mip mapping in the albedo texture when generate them
		if (!m_skipmips && m_specularTexture)
		{
			GLTexture2D* texture2DImpl = reinterpret_cast<GLTexture2D*>(m_specularTexture->getHWTexture());
			glGenerateTextureMipmapEXT(texture2DImpl->getHandle(), GL_TEXTURE_2D);
		}
#endif
	}

	void Material::releaseHw()
	{
		if (m_albedoSampler)
		{
			mem_delete(m_albedoSampler);
			m_albedoSampler = nullptr;
		}
	}

	///////////////////////////////////////////////////////
	// Generic Material
	///////////////////////////////////////////////////////

	GenericMaterial::GenericMaterial()
	{
	}

	GenericMaterial::~GenericMaterial()
	{
	}

	void GenericMaterial::registerObject()
	{
		TypeManager::getInstance()->registerObject<GenericMaterial>();
	}

	///////////////////////////////////////////////////////
	// Layered and masked material
	///////////////////////////////////////////////////////


	LayeredMaskedMaterial::LayeredMaskedMaterial()
	{
	}

	LayeredMaskedMaterial::~LayeredMaskedMaterial()
	{
	}

	void LayeredMaskedMaterial::registerObject()
	{
		TypeManager::getInstance()->registerObject<LayeredMaskedMaterial>();
	}

	///////////////////////////////////////////////////////
	// Null material
	///////////////////////////////////////////////////////

	NullMaterial::NullMaterial()
	{
	}

	NullMaterial::~NullMaterial()
	{
	}

	void NullMaterial::registerObject()
	{
		TypeManager::getInstance()->registerObject<NullMaterial>();
	}
}
