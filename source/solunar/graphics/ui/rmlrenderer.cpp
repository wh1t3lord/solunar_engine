#include "graphicspch.h"
#if 0
#include "graphics/ui/rmlrenderer.h"

#include "graphics/core/buffer.h"
#include "graphics/core/texture.h"
#include "graphics/core/device.h"

#include "graphics/texturemap.h"
#include "graphics/material.h"
#include "graphics/shaderprogram.h"
#include "graphics/ShaderProgramManager.h"

#include "engine/content/contentmanager.h"
#include "engine/camera.h"

#include <glad/glad.h>

namespace solunar
{

struct RmlTextureHandle
{
	std::shared_ptr<TextureMap> m_textureMap;
};

struct RmlGeometryHandle
{
	IBufferBase* m_vertexBuffer;
	IBufferBase* m_indexBuffer;
	Rml::TextureHandle m_textureHandle;
	int m_numVertices;
	int m_numIndices;
};

ISamplerState* g_defaultRmlUISampler;

RmlRenderer* g_rmlRenderer = nullptr;

RmlRenderer::RmlRenderer() :
	m_constantBuffer(nullptr),
	m_shaderProgram(nullptr)
{
}

RmlRenderer::~RmlRenderer()
{
}

void RmlRenderer::Init()
{
	Core::Msg("[graphics]: initializing rml renderer");

	RmlConstants constants;
	memset(&constants, 0, sizeof(constants));

	// Create constant buffer
	BufferDesc bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.m_bufferType = BufferType::ConstantBuffer;
	bufferDesc.m_bufferAccess = BufferAccess::Stream;
	bufferDesc.m_bufferMemorySize = sizeof(RmlConstants);

	SubresourceDesc subresourceDesc;
	memset(&subresourceDesc, 0, sizeof(subresourceDesc));
	subresourceDesc.m_memory = &constants;

	m_constantBuffer = g_renderDevice->CreateBuffer(bufferDesc, subresourceDesc);

	// Create sampler state

	SamplerDesc samplerDesc;
	memset(&samplerDesc, 0, sizeof(samplerDesc));
	samplerDesc.m_minFilter = TextureFilter::Linear;
	samplerDesc.m_magFilter = TextureFilter::Linear;
	samplerDesc.m_wrapS = TextureWrap::ClampToEdge;
	samplerDesc.m_wrapT = TextureWrap::ClampToEdge;
	samplerDesc.m_anisotropyLevel = 1.0f;

	g_defaultRmlUISampler = g_renderDevice->CreateSamplerState(samplerDesc);

	// Load shader
	m_shaderProgram = g_shaderManager->CreateShaderProgram("2d_ui.vsh", "2d_ui.psh");
}

void RmlRenderer::Shutdown()
{
	if (g_defaultRmlUISampler)
	{
		mem_delete(g_defaultRmlUISampler);
		g_defaultRmlUISampler = nullptr;
	}

	if (m_constantBuffer)
	{
		mem_delete(m_constantBuffer);
		m_constantBuffer = nullptr;
	}
}

void RmlRenderer::BeginFrame()
{
#if 0
	glClearStencil(0);
	glClearColor(0, 0, 0, 1);

	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, GLuint(-1));
	glStencilMask(GLuint(-1));
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
#endif
}

void RmlRenderer::EndFrame()
{
}

void RmlRenderer::RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation)
{
	Rml::CompiledGeometryHandle geometry = CompileGeometry(vertices, num_vertices, indices, num_indices, texture);
	if (geometry)
	{
		RenderCompiledGeometry(geometry, translation);
		ReleaseCompiledGeometry(geometry);
	}
}

Rml::CompiledGeometryHandle RmlRenderer::CompileGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture)
{
	RmlGeometryHandle* geometryHandle = mem_new<RmlGeometryHandle>();

	// vertex buffer creation
	{
		BufferDesc bufferDesc = {};
		bufferDesc.m_bufferType = BufferType::VertexBuffer;
		bufferDesc.m_bufferAccess = BufferAccess::Static;
		bufferDesc.m_bufferMemorySize = sizeof(Rml::Vertex) * num_vertices;

		SubresourceDesc subresourceDesc = {};
		subresourceDesc.m_memory = vertices;
		//subresourceDesc.m_memoryPitch = 
		geometryHandle->m_vertexBuffer = g_renderDevice->CreateBuffer(bufferDesc, subresourceDesc);
	}

	// index buffer creation
	{
		BufferDesc bufferDesc = {};
		bufferDesc.m_bufferType = BufferType::IndexBuffer;
		bufferDesc.m_bufferAccess = BufferAccess::Static;
		bufferDesc.m_bufferMemorySize = sizeof(int) * num_indices;

		SubresourceDesc subresourceDesc = {};
		subresourceDesc.m_memory = indices;

		geometryHandle->m_indexBuffer = g_renderDevice->CreateBuffer(bufferDesc, subresourceDesc);
	}

	geometryHandle->m_textureHandle = texture;
	geometryHandle->m_numVertices = num_vertices;
	geometryHandle->m_numIndices = num_indices;
	return (Rml::CompiledGeometryHandle)geometryHandle;
}

void RmlRenderer::RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry, const Rml::Vector2f& translation)
{
	RmlGeometryHandle* geometryHandle = (RmlGeometryHandle*)geometry;
	ASSERT(geometryHandle);

	View* view = CameraProxy::GetInstance()->GetView();
	Rml::Matrix4f projection = Rml::Matrix4f::ProjectOrtho(0, (float)view->getWidth(), (float)view->getHeight(), 0, -10000, 10000);

	RmlConstants* constants = (RmlConstants*)m_constantBuffer->Map(BufferMapping::WriteOnly);

	// translation
	constants->m_translate.x = translation.x;
	constants->m_translate.y = translation.y;

	// projection matrix
	constants->m_projectionMatrix = glm::make_mat4(projection.data());

	m_constantBuffer->Unmap();

	// Initialize device state

	g_shaderManager->SetShaderProgram(m_shaderProgram);

	VertexFormat vtxf;
	vtxf.addPosition_Float2();
	vtxf.addColor_DWORD();
	vtxf.addTexcoord();
	//g_renderDevice->setVertexFormat(&vtxf);

	g_renderDevice->SetVertexBuffer(geometryHandle->m_vertexBuffer, sizeof(Rml::Vertex), 0);
	g_renderDevice->SetIndexBuffer(geometryHandle->m_indexBuffer);
	g_renderDevice->SetConstantBufferIndex(0, m_constantBuffer);

	// get texture handle

	RmlTextureHandle* textureHandle = (RmlTextureHandle*)geometryHandle->m_textureHandle;
	ASSERT(textureHandle);
	ASSERT(textureHandle->m_textureMap);
	ASSERT(textureHandle->m_textureMap->getHWTexture());

	g_renderDevice->SetTexture2D(0, textureHandle->m_textureMap->getHWTexture());
	g_renderDevice->SetSamplerState(0, g_defaultRmlUISampler);

	// render
	g_renderDevice->DrawIndexed(PM_TriangleList, 0, geometryHandle->m_numIndices);
}

void RmlRenderer::ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry)
{
	RmlGeometryHandle* geometryHandle = (RmlGeometryHandle*)geometry;
	ASSERT(geometryHandle);

	if (geometryHandle->m_indexBuffer)
	{
		mem_delete(geometryHandle->m_indexBuffer);
		geometryHandle->m_indexBuffer = nullptr;
	}

	if (geometryHandle->m_vertexBuffer)
	{
		mem_delete(geometryHandle->m_vertexBuffer);
		geometryHandle->m_vertexBuffer = nullptr;
	}

	mem_delete(geometryHandle);
}

bool RmlRenderer::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source)
{
	// Create texture map
	RmlTextureHandle* textureHandle = mem_new<RmlTextureHandle>();
	textureHandle->m_textureMap = ContentManager::GetInstance()->LoadTexture(source);

	texture_handle = (Rml::TextureHandle)textureHandle;

	return true;
}

bool RmlRenderer::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions)
{
	// Initialize 2d texture
	TextureDesc textureDesc;
	memset(&textureDesc, 0, sizeof(TextureDesc));
	textureDesc.m_width = source_dimensions.x;
	textureDesc.m_height = source_dimensions.y;
	textureDesc.m_textureType = TextureType::Texture2D;
	textureDesc.m_mipmapLevel = 1;
	textureDesc.m_format = ImageFormat::RGBA32;

	SubresourceDesc textureSubresourceDesc;
	memset(&textureSubresourceDesc, 0, sizeof(textureSubresourceDesc));
	textureSubresourceDesc.m_memory = (void*)source;
	textureSubresourceDesc.m_memoryPitch = source_dimensions.x * 4;

	ITexture2D* texture2d = g_renderDevice->CreateTexture2D(textureDesc, textureSubresourceDesc);

	// Create texture map
	RmlTextureHandle* textureHandle = mem_new<RmlTextureHandle>();
	textureHandle->m_textureMap = TextureMap::create2DFromSource(texture2d, textureDesc);

	texture_handle = (Rml::TextureHandle)textureHandle;

	return true;
}

void RmlRenderer::ReleaseTexture(Rml::TextureHandle texture)
{
	RmlTextureHandle* textureHandle = (RmlTextureHandle*)texture;

	// Need check for it because some textures can be cached by Content Manager
	if (textureHandle->m_textureMap->isManualCreated())
		textureHandle->m_textureMap->Release();

	// Release texture handle
	textureHandle->m_textureMap = nullptr;

	// Destroy backend handle
	mem_delete(textureHandle);
	textureHandle = nullptr;
}

void RmlRenderer::EnableScissorRegion(bool enable)
{
}

void RmlRenderer::SetScissorRegion(int x, int y, int width, int height)
{
}

void RmlRenderer::SetTransform(const Rml::Matrix4f* transform)
{
	ASSERT(transform);
	m_transform = glm::make_mat4(transform->data());
}

}
#endif