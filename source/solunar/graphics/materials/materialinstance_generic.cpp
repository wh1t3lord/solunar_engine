#include "graphicspch.h"
#include "graphics/materials/materialinstance_generic.h"
#include "graphics/ShaderProgramManager.h"
#include "graphics/model.h"
#include "graphics/animatedmodel.h"

namespace solunar
{

static InputLayoutDesc s_vertexInputLayout[] =
{
	{ "POSITION", 0, ImageFormat::RGB32F, 0, (UINT)offsetof(Vertex, m_position), INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, ImageFormat::RGB32F, 0, (UINT)offsetof(Vertex, m_normal), INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, ImageFormat::RG32F, 0, (UINT)offsetof(Vertex, m_texcoord0), INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, ImageFormat::RGB32F, 0, (UINT)offsetof(Vertex, m_tangent), INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL", 0, ImageFormat::RGB32F, 0, (UINT)offsetof(Vertex, m_bitangent), INPUT_PER_VERTEX_DATA, 0 }
};

static InputLayoutDesc s_animatedVertexInputLayout[] =
{
	{ "POSITION", 0, ImageFormat::RGB32F, 0, (UINT)offsetof(AnimatedVertex, m_position), INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, ImageFormat::RGB32F, 0, (UINT)offsetof(AnimatedVertex, m_normal), INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, ImageFormat::RG32F, 0, (UINT)offsetof(AnimatedVertex, m_texcoord), INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, ImageFormat::RGB32F, 0, (UINT)offsetof(AnimatedVertex, m_tangent), INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL", 0, ImageFormat::RGB32F, 0, (UINT)offsetof(AnimatedVertex, m_bitangent), INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDWEIGHT", 0, ImageFormat::RGBA32F, 0, (UINT)offsetof(AnimatedVertex, m_weights), INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDINDICES", 0, ImageFormat::RGBA32F, 0, (UINT)offsetof(AnimatedVertex, m_boneIDs), INPUT_PER_VERTEX_DATA, 0 }
};

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

IShaderProgram* MaterialInstance_Generic::ms_pixelVariations[VertexFactory_Count][512];

MaterialInstance_Generic::MaterialInstance_Generic()
{
	// #TODO: MAKE NORMAL SHADER PRECACHE
	static bool isInited = false;
	if (!isInited)
	{
		getShaderProgramVariation(VertexFactory_StaticMesh, PixelVariation_Lit);
		getShaderProgramVariation(VertexFactory_StaticMesh, PixelVariation_Unlit);
		getShaderProgramVariation(VertexFactory_SkinnedMesh, PixelVariation_Lit);
		getShaderProgramVariation(VertexFactory_SkinnedMesh, PixelVariation_Unlit);
		isInited = true;
	}
}

MaterialInstance_Generic::~MaterialInstance_Generic()
{
}

IShaderProgram* MaterialInstance_Generic::getShaderProgramVariation(VertexFactory vertexFactory, uint32_t pixelVariation)
{
	Assert(vertexFactory <= VertexFactory_Count);

	if (ms_pixelVariations[vertexFactory][pixelVariation])
		return ms_pixelVariations[vertexFactory][pixelVariation];

	std::string variationName = getPixelVariationName("materialinstance_generic", pixelVariation);
	Core::Msg("Graphics: Generation variation '%s' for MaterialInstance_Generic", variationName.c_str());

	std::string defines = getPixelVariationDefine(pixelVariation);

	IShaderProgram* shaderProgram = nullptr;

	switch (vertexFactory)
	{
	case VertexFactory_StaticMesh:
		shaderProgram = g_shaderManager->CreateShaderProgram(
			"materialinstance_generic.hlsl",
			"materialinstance_generic.hlsl",
			defines.c_str(),
			s_vertexInputLayout,
			sizeof(s_vertexInputLayout) / sizeof(s_vertexInputLayout[0]));
		break;
	case VertexFactory_SkinnedMesh:
		defines += "SKINNED\n";

		shaderProgram = g_shaderManager->CreateShaderProgram(
			"materialinstance_generic.hlsl",
			"materialinstance_generic.hlsl",
			defines.c_str(),
			s_animatedVertexInputLayout,
			sizeof(s_animatedVertexInputLayout) / sizeof(s_animatedVertexInputLayout[0]));
		break;
	//case VertexFactory_Billboard:
	//	break;
	}

	Assert2(shaderProgram, "Unknowed vertex factory");

	ms_pixelVariations[vertexFactory][pixelVariation] = shaderProgram;

	return shaderProgram;
}

}