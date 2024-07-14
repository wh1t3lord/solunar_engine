#include "graphicspch.h"
#include "graphics/materials/materialinstance_generic.h"
#include "graphics/ShaderProgramManager.h"
#include "graphics/model.h"

namespace engine
{

std::unordered_map<std::string, IShaderProgram*> g_materialInstancePixelVariations;

std::string getPixelVariationDefine(uint32_t pixelVariation)
{
	std::string defines;
	
	if (pixelVariation & PixelVariation_Unlit)
		defines.append("UNLIT\n");

	return defines;
}

MaterialInstance_Generic::MaterialInstance_Generic()
{
	//m_staticMeshShader = g_shaderManager->createShaderProgram("materialinstance_generic.vsh",
	//	"materialinstance_generic.psh");

	//m_skinnedMeshShader = g_shaderManager->createShaderProgram("materialinstance_generic.vsh",
	//	"materialinstance_generic.psh",
	//	"#define SKINNED\n");

	//m_particleShader = g_shaderManager->createShaderProgram("materialinstance_generic.vsh",
	//	"materialinstance_generic.psh",
	//	"#define PARTICLE\n");
}

MaterialInstance_Generic::~MaterialInstance_Generic()
{
}

IShaderProgram* MaterialInstance_Generic::getShaderProgram_StaticVertexFactory_Variation(uint32_t pixelVariation)
{
	std::string shaderName = "materialinstance_generic";
	if (pixelVariation & PixelVariation_Unlit)
		shaderName += "_unlit";
	if (pixelVariation & PixelVariation_Lit)
		shaderName += "_lit";

	auto it = g_materialInstancePixelVariations.find(shaderName);
	if (it != g_materialInstancePixelVariations.end())
		return it->second;

	Core::msg("Graphics: Generation variation '%s' for MaterialInstance_Generic", shaderName.c_str());

	std::string defines = getPixelVariationDefine(pixelVariation);

	InputLayoutDesc inputLayout[] =
	{
		{ "POSITION", 0, ImageFormat::RGBA32F, 0, (UINT)offsetof(Vertex, m_position), INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, ImageFormat::RGBA32F, 0, (UINT)offsetof(Vertex, m_normal),  INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, ImageFormat::RG32F, 0, (UINT)offsetof(Vertex, m_texcoord0),  INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, ImageFormat::RGBA32F, 0, (UINT)offsetof(Vertex, m_tangent),  INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, ImageFormat::RGBA32F, 0, (UINT)offsetof(Vertex, m_bitangent),  INPUT_PER_VERTEX_DATA, 0 }
	};

	IShaderProgram* shaderProgram = g_shaderManager->createShaderProgram(
		"materialinstance_generic.vsh",
		"materialinstance_generic.psh",
		defines.c_str(),
		inputLayout,
		sizeof(inputLayout) / sizeof(inputLayout[0]));

	g_materialInstancePixelVariations.emplace(shaderName, shaderProgram);

	return shaderProgram;
}

}