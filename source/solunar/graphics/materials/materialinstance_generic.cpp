#include "graphicspch.h"
#include "graphics/materials/materialinstance_generic.h"
#include "graphics/ShaderProgramManager.h"
#include "graphics/model.h"
#include "graphics/animatedmodel.h"

namespace engine
{

static std::unordered_map<std::string, IShaderProgram*> m_pixelVariations[VertexFactory_Count];

std::string getPixelVariationDefine(uint32_t pixelVariation)
{
	std::string defines;
	
	if (pixelVariation & PixelVariation_Unlit)
		defines.append("UNLIT\n");

	return defines;
}

std::string getPixelVariationName(const std::string& instanceName, uint32_t pixelVariation)
{
	std::string shaderName = instanceName;
	if (pixelVariation & PixelVariation_Unlit)
		shaderName += "_unlit";
	if (pixelVariation & PixelVariation_Lit)
		shaderName += "_lit";
	return shaderName;
}

MaterialInstance_Generic::MaterialInstance_Generic()
{
}

MaterialInstance_Generic::~MaterialInstance_Generic()
{
}

IShaderProgram* MaterialInstance_Generic::getShaderProgramVariation(VertexFactory vertexFactory, uint32_t pixelVariation)
{
	Assert(vertexFactory <= VertexFactory_Count);

	std::string variationName = getPixelVariationName("materialinstance_generic", pixelVariation);
	auto& pixelVariationMap = m_pixelVariations[vertexFactory];

	auto it = pixelVariationMap.find(variationName);
	if (it != pixelVariationMap.end())
		return it->second;

	Core::msg("Graphics: Generation variation '%s' for MaterialInstance_Generic", variationName.c_str());

	std::string defines = getPixelVariationDefine(pixelVariation);

	IShaderProgram* shaderProgram = nullptr;

	switch (vertexFactory)
	{
	case VertexFactory_StaticMesh:
		shaderProgram = g_shaderManager->createShaderProgram(
			"materialinstance_generic.vsh",
			"materialinstance_generic.psh",
			defines.c_str(),
			g_vertexInputLayout,
			sizeof(g_vertexInputLayout) / sizeof(g_vertexInputLayout[0]));
		break;
	case VertexFactory_SkinnedMesh:
		defines += "SKINNED\n";

		shaderProgram = g_shaderManager->createShaderProgram(
			"materialinstance_generic.vsh",
			"materialinstance_generic.psh",
			defines.c_str(),
			g_animatedVertexInputLayout,
			sizeof(g_animatedVertexInputLayout) / sizeof(g_animatedVertexInputLayout[0]));
		break;
	//case VertexFactory_Billboard:
	//	break;
	}

	Assert2(shaderProgram, "Unknowed vertex factory");

	pixelVariationMap.emplace(variationName, shaderProgram);

	return shaderProgram;
}

}