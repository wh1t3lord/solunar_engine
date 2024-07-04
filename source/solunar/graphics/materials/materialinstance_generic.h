#ifndef GRAPHICS_MATERIALS_MATERIALINSTANCE_GENERIC_H
#define GRAPHICS_MATERIALS_MATERIALINSTANCE_GENERIC_H

#include "graphics/materials/materialinstance.h"

namespace engine
{

class IShaderProgram;

class MaterialInstance_Generic : public MaterialInstance
{
public:
	MaterialInstance_Generic();
	~MaterialInstance_Generic();

	IShaderProgram* getStaticMeshShaderProgram() override { return m_staticMeshShader; }
	IShaderProgram* getSkinnedShaderProgram() override { return m_skinnedMeshShader; }
	IShaderProgram* getParticleShaderProgram() override { return m_particleShader; }

	IShaderProgram* getShaderProgram_StaticVertexFactory_Variation(uint32_t pixelVariation);

private:
	IShaderProgram* m_staticMeshShader;
	IShaderProgram* m_skinnedMeshShader;
	IShaderProgram* m_particleShader;

};

}

#endif // !GRAPHICS_MATERIALS_MATERIALINSTANCE_GENERIC_H
