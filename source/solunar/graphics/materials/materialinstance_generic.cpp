#include "graphicspch.h"
#include "graphics/materials/materialinstance_generic.h"
#include "graphics/ShaderProgramManager.h"

namespace engine
{

MaterialInstance_Generic::MaterialInstance_Generic()
{
	m_staticMeshShader = g_shaderManager->createShaderProgram("materialinstance_generic.vsh",
		"materialinstance_generic.psh");

	m_skinnedMeshShader = g_shaderManager->createShaderProgram("materialinstance_generic.vsh",
		"materialinstance_generic.psh",
		"#define SKINNED\n");

	m_particleShader = g_shaderManager->createShaderProgram("materialinstance_generic.vsh",
		"materialinstance_generic.psh",
		"#define PARTICLE\n");
}

MaterialInstance_Generic::~MaterialInstance_Generic()
{
}

}