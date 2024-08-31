#include "graphicspch.h"

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_RASTERIZER_VERSION 1
#include "graphics/fontmanager.h"

#include "core/file/filesystem.h"
#include "core/file/contentdevice.h"

#include "graphics/core/device.h"
#include "graphics/core/texture.h"
#include "graphics/core/buffer.h"
#include "graphics/core/statemanager.h"

// renderer
#include "graphics/view.h"
#include "graphics/renderer.h"

// Shader system
#include "graphics/shaderprogram.h"
#include "graphics/ShaderProgramManager.h"

#include "engine/camera.h"

#include <utility>

namespace solunar
{

IFontManager* g_fontManager = nullptr;

FontManager::FontManager()
{
	m_systemFont = nullptr;
	m_textureSampler = nullptr;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_constantBuffer = nullptr;
	m_shaderProgram = nullptr;
	m_rasterizerState = nullptr;
	m_depthStencilState = nullptr;

	m_drawStrings.reserve(1024);
}

FontManager::~FontManager()
{
}

void FontManager::Initialize()
{
	Assert2(g_renderDevice, "Please Initialize font manager after core graphics initialization");

	// Load system font
	m_systemFont = CreateFont("textures/ui/system.ttf", 16.0f);

	InitPrivate();
}

void FontManager::InitPrivate()
{
	// create sampler state for font texture

	SamplerDesc fontSamplerDesc;
	memset(&fontSamplerDesc, 0, sizeof(fontSamplerDesc));
	fontSamplerDesc.m_minFilter = TextureFilter::Nearest;
	fontSamplerDesc.m_magFilter = TextureFilter::Nearest;
	fontSamplerDesc.m_wrapS = TextureWrap::ClampToEdge;
	fontSamplerDesc.m_wrapT = TextureWrap::ClampToEdge;
	fontSamplerDesc.m_anisotropyLevel = 1.0f;

	m_textureSampler = g_renderDevice->CreateSamplerState(fontSamplerDesc);

	// create characters buffer

	BufferDesc charBufferDesc;
	memset(&charBufferDesc, 0, sizeof(charBufferDesc));
	charBufferDesc.m_bufferType = BufferType::VertexBuffer;
	charBufferDesc.m_bufferAccess = BufferAccess::Stream;
	charBufferDesc.m_bufferMemorySize = kMaxFontVBSize;

	SubresourceDesc charSubresourceDesc;
	memset(&charSubresourceDesc, 0, sizeof(charSubresourceDesc));

	// NOTE: We create dynamic vertex buffer without any data.
	//       And we will fill buffer at every time when we need to draw some sentence.
	m_vertexBuffer = g_renderDevice->CreateBuffer(charBufferDesc, charSubresourceDesc);

	// create characters index buffer

	BufferDesc charIndexBufferDesc;
	memset(&charBufferDesc, 0, sizeof(charIndexBufferDesc));
	charIndexBufferDesc.m_bufferType = BufferType::IndexBuffer;
	charIndexBufferDesc.m_bufferAccess = BufferAccess::Stream;
	charIndexBufferDesc.m_bufferMemorySize = kMaxFontIBSize;

	// NOTE: We create dynamic index buffer without any data.
	//       And we will fill buffer at every time when we need to draw some sentence.
	m_indexBuffer = g_renderDevice->CreateBuffer(charIndexBufferDesc, charSubresourceDesc);

	// create constant buffer
	BufferDesc constantBufferDesc;
	memset(&constantBufferDesc, 0, sizeof(constantBufferDesc));
	constantBufferDesc.m_bufferType = BufferType::ConstantBuffer;
	constantBufferDesc.m_bufferAccess = BufferAccess::Stream;
	constantBufferDesc.m_bufferMemorySize = sizeof(glm::mat4);

	SubresourceDesc constantSubresorceDesc;
	memset(&constantSubresorceDesc, 0, sizeof(constantSubresorceDesc));

	m_constantBuffer = g_renderDevice->CreateBuffer(constantBufferDesc, constantSubresorceDesc);

	// Create 2d font shader
	InputLayoutDesc layout[] =
	{
		{ "POSITION", 0, ImageFormat::RG32F,   0, (UINT)offsetof(FontVertex, position), INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, ImageFormat::RG32F,   0, (UINT)offsetof(FontVertex, texcoord),  INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, ImageFormat::RGBA32F,   0, (UINT)offsetof(FontVertex, color),  INPUT_PER_VERTEX_DATA, 0 },
	};

	m_shaderProgram = g_shaderManager->CreateShaderProgram("2d_font.vsh", "2d_font.psh", nullptr,
		layout, sizeof(layout) / sizeof(layout[0]));

	// create rasterizer state

	RasterizerStateDesc rasterizerState;
	memset(&rasterizerState, 0, sizeof(rasterizerState));
	rasterizerState.m_cullMode = CullMode::Back;
	rasterizerState.m_frontCCW = false;
	rasterizerState.m_fillMode = FillMode::Solid;

	m_rasterizerState = g_stateManager->CreateRasterizerState(rasterizerState);

	// create depth stencil test
	DepthStencilDesc desc = {};
	desc.m_depthEnable = false;
	desc.m_depthWriteMask = DEPTH_WRITE_MASK_ALL;
	desc.m_depthFunc = COMPARISON_ALWAYS;
	desc.m_stencilEnable = false;
	desc.m_frontFace.m_stencilFailOp = desc.m_frontFace.m_stencilDepthFailOp = desc.m_frontFace.m_stencilPassOp = STENCIL_OP_KEEP;
	desc.m_frontFace.m_stencilFunc = COMPARISON_ALWAYS;
	desc.m_backFace = desc.m_frontFace;
	m_depthStencilState = g_stateManager->CreateDepthStencilState(desc);


	// Create the blending setup
	BlendStateDesc blendStateDesc = {};
	blendStateDesc.m_alphaToCoverageEnable = false;
	blendStateDesc.m_renderTarget[0].m_blendEnable = true;
	blendStateDesc.m_renderTarget[0].m_srcBlend = BLEND_SRC_ALPHA;
	blendStateDesc.m_renderTarget[0].m_destBlend = BLEND_INV_SRC_ALPHA;
	blendStateDesc.m_renderTarget[0].m_blendOp = BLEND_OP_ADD;
	blendStateDesc.m_renderTarget[0].m_srcBlendAlpha = BLEND_ONE;
	blendStateDesc.m_renderTarget[0].m_destBlendAlpha = BLEND_INV_SRC_ALPHA;
	blendStateDesc.m_renderTarget[0].m_blendOpAlpha = BLEND_OP_ADD;
	blendStateDesc.m_renderTarget[0].m_renderTargetWriteMask = COLOR_WRITE_ENABLE_ALL;
	m_pBlendState = g_stateManager->CreateBlendState(blendStateDesc);
}

void FontManager::FlushPrimitives()
{
	Assert2(g_renderer, "Called before renderer initialization.");
	Assert(m_vertexBuffer);

	if (m_drawStrings.empty())
		return;

	// bind texture and their sampler
	g_renderDevice->SetSamplerState(0, m_textureSampler);

	// setup shader and others stuff
	g_shaderManager->SetShaderProgram(m_shaderProgram);

	// build orthogonal matrix

	// get current view
	View* view = CameraProxy::GetInstance()->GetView();

	// calculate ortho matrix based on current view
	glm::mat4 proj = glm::ortho(0.0f, (float)view->m_width, 0.0f, (float)view->m_height, 0.1f, 100.0f);

	// update constant buffer
	void* constantBufferData = m_constantBuffer->Map(BufferMapping::WriteOnly);
	memcpy(constantBufferData, &proj[0], sizeof(proj));
	m_constantBuffer->Unmap();

	// setup it to shader
	g_renderDevice->SetConstantBufferIndex(0, m_constantBuffer);

	// enable rasterizer state
	g_stateManager->SetRasterizerState(m_rasterizerState);

	// disable depth test
	g_stateManager->SetDepthStencilState(m_depthStencilState, 0);

	// Setup blend state
	const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
	g_stateManager->SetBlendState(m_pBlendState, blend_factor, 0xffffffff);

	// bind vertex buffer
	g_renderDevice->SetVertexBuffer(m_vertexBuffer, sizeof(FontVertex), 0);

	// bind index buffer
	g_renderDevice->SetIndexBuffer(m_indexBuffer, true);

	// draw font strings
	for (auto& it : m_drawStrings)
	{
		// bind texture 
		g_renderDevice->SetTexture2D(0, it.m_font->m_fontTexture);

		// map our text buffer
		FontVertex* fontVertices = (FontVertex*)m_vertexBuffer->Map(BufferMapping::WriteOnly);

		// map our index buffer
		uint16_t* fontIndices = (uint16_t*)m_indexBuffer->Map(BufferMapping::WriteOnly);

		uint32_t numVertices = 0;
		uint32_t numIndices = 0;
		uint16_t currentIndex = 0;
		size_t stringLength = it.m_string.length();

		for (int i = 0; i < stringLength; i++)
		{
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(it.m_font->m_fontChars, 512, 512, it.m_string[i] - 32, &it.m_x, &it.m_y, &q, 1);

			fontIndices[numIndices] = currentIndex;
			fontIndices[numIndices + 1] = currentIndex + 1;
			fontIndices[numIndices + 2] = currentIndex + 2;
			fontIndices[numIndices + 3] = currentIndex;
			fontIndices[numIndices + 4] = currentIndex + 2;
			fontIndices[numIndices + 5] = currentIndex + 3;

			// position
			fontVertices[numVertices + 0].position = glm::vec2(q.x0, (float)view->m_height - q.y0);
			fontVertices[numVertices + 1].position = glm::vec2(q.x1, (float)view->m_height - q.y0);
			fontVertices[numVertices + 2].position = glm::vec2(q.x1, (float)view->m_height - q.y1);
			fontVertices[numVertices + 3].position = glm::vec2(q.x0, (float)view->m_height - q.y1);

			// color
			fontVertices[numVertices + 0].color = it.m_color;
			fontVertices[numVertices + 1].color = it.m_color;
			fontVertices[numVertices + 2].color = it.m_color;
			fontVertices[numVertices + 3].color = it.m_color;

			// texcoord
			fontVertices[numVertices + 0].texcoord = glm::vec2(q.s0, q.t0);
			fontVertices[numVertices + 1].texcoord = glm::vec2(q.s1, q.t0);
			fontVertices[numVertices + 2].texcoord = glm::vec2(q.s1, q.t1);
			fontVertices[numVertices + 3].texcoord = glm::vec2(q.s0, q.t1);

			currentIndex += 4;
			numIndices += 6;
			numVertices += 4;
		}

		// unmap
		m_indexBuffer->Unmap();
		m_vertexBuffer->Unmap();

		// draw
		g_renderDevice->DrawIndexed(PM_TriangleList, 0, numIndices, 0);
	}

	m_drawStrings.clear();
	//m_drawStrings.resize(0);
}

void FontManager::Shutdown()
{
	for (auto& it : m_fonts)
	{
		mem_delete(it.second);
	}

	m_fonts.clear();

	if (m_constantBuffer)
	{
		mem_delete(m_constantBuffer);
		m_constantBuffer = nullptr;
	}

	if (m_indexBuffer)
	{
		mem_delete(m_indexBuffer);
		m_indexBuffer = nullptr;
	}

	if (m_vertexBuffer)
	{
		mem_delete(m_vertexBuffer);
		m_vertexBuffer = nullptr;
	}

	if (m_textureSampler)
	{
		mem_delete(m_textureSampler);
		m_textureSampler = nullptr;
	}
}

IFont* FontManager::CreateFont(const char* filename, float size)
{
	m_fonts[filename] = mem_new<FontImpl>();
	m_fonts[filename]->Create(filename, size);
	return m_fonts[filename];
}

void FontManager::DrawFontText(IFont* font, const char* text, float x, float y, const glm::vec4& color)
{
	StringDrawInfo drawInfo;
	drawInfo.m_font = (FontImpl*)font;
	drawInfo.m_string = text;
	drawInfo.m_x = x;
	drawInfo.m_y = y;
	drawInfo.m_color = color;
	m_drawStrings.push_back(drawInfo);
}

void FontManager::DrawSystemFont(const char* text, float x, float y, const glm::vec4& color)
{
	m_systemFont->DrawText(text, x, y, color);
}

// Font implementation

FontImpl::FontImpl() :
	m_fontTexture(nullptr)
{
	memset(m_fontChars, 0, sizeof(m_fontChars));
}

FontImpl::~FontImpl()
{
	Destroy();
}

void FontImpl::Create(const char* filename, float size)
{
	Assert2(g_renderDevice, "Please Initialize font manager after core graphics initialization");

	// Load system font
	DataStreamPtr stream = g_contentManager->OpenStream(filename);
	stream->Seek(Seek_End, 0);
	long fileLength = stream->Tell();
	stream->Seek(Seek_Begin, 0);

	uint8_t* ttfBuffer = new uint8_t[fileLength];
	stream->Read(ttfBuffer, fileLength);

	uint8_t* tmpBitmap = new uint8_t[512 * 512]; //res of the bitmap
	stbtt_BakeFontBitmap(ttfBuffer, 0, size, tmpBitmap, 512, 512, 32, 96, m_fontChars);

	TextureDesc textureDesc;
	memset(&textureDesc, 0, sizeof(textureDesc));
	textureDesc.m_textureType = TextureType::Texture2D;
	textureDesc.m_width = 512;
	textureDesc.m_height = 512;
	textureDesc.m_mipmapLevel = 0;
	textureDesc.m_format = ImageFormat::R32;

	SubresourceDesc subresourceDesc;
	memset(&subresourceDesc, 0, sizeof(subresourceDesc));
	subresourceDesc.m_memory = tmpBitmap;
	subresourceDesc.m_memoryPitch = 512;

	m_fontTexture = g_renderDevice->CreateTexture2D(textureDesc, subresourceDesc);

	delete[] tmpBitmap;
	delete[] ttfBuffer;
}

void FontImpl::Destroy()
{
	if (m_fontTexture)
	{
		mem_delete(m_fontTexture);
		m_fontTexture = nullptr;
	}
}

void FontImpl::DrawText(const char* text, float x, float y, const glm::vec4& color)
{
	Assert(g_fontManager);
	g_fontManager->DrawFontText(this, text, x, y, color);
}

}