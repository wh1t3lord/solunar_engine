#ifndef GRAPHICS_UI_RMLRENDERER_H
#define GRAPHICS_UI_RMLRENDERER_H

#if 0

#include <RmlUi/Core/RenderInterface.h>

namespace solunar
{

class IShaderProgram;
class IBufferBase;

struct RmlConstants
{
	glm::vec4 m_translate;
	glm::mat4 m_projectionMatrix;
};

class RmlRenderer : public Rml::RenderInterface
{
public:
	RmlRenderer();
	~RmlRenderer();

	void Init();
	void Shutdown();

	void BeginFrame();
	void EndFrame();

	// Rml::RenderInterface interface

	void RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation) override;

	Rml::CompiledGeometryHandle CompileGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture) override;
	void RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry, const Rml::Vector2f& translation) override;
	void ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry) override;

	bool LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source) override;
	bool GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions) override;
	void ReleaseTexture(Rml::TextureHandle texture) override;

	void EnableScissorRegion(bool enable) override;
	void SetScissorRegion(int x, int y, int width, int height) override;

	void SetTransform(const Rml::Matrix4f* transform) override;

private:
	glm::mat4 m_transform;
	IBufferBase* m_constantBuffer;
	IShaderProgram* m_shaderProgram;

};

extern RmlRenderer* g_rmlRenderer;

}

#endif

#endif // !GRAPHICS_UI_RMLRENDERER_H
