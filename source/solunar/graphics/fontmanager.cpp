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

namespace engine
{

IFontManager* g_fontManager = nullptr;

FontManager::FontManager()
{
	memset(m_systemFontChars, 0, sizeof(m_systemFontChars));
	m_systemFontTexture = nullptr;
	m_textureSampler = nullptr;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_constantBuffer = nullptr;
	m_shaderProgram = nullptr;
	m_rasterizerState = nullptr;
	m_depthStencilState = nullptr;
}

FontManager::~FontManager()
{
}

void FontManager::initialize()
{
	Assert2(g_renderDevice, "Please initialize font manager after core graphics initialization");

	// load system font
	DataStreamPtr systemFontStream = g_contentManager->openStream("textures/ui/system.ttf");
	systemFontStream->seek(Seek_End, 0);
	long systemFontFileLength = systemFontStream->tell();
	systemFontStream->seek(Seek_Begin, 0);

	uint8_t* ttfBuffer = new uint8_t[systemFontFileLength];
	systemFontStream->read(ttfBuffer, systemFontFileLength);

	uint8_t* tmpBitmap = new uint8_t[512 * 512]; //res of the bitmap
	stbtt_BakeFontBitmap(ttfBuffer, 0, 16.0f, tmpBitmap, 512, 512, 32, 96, m_systemFontChars);

	TextureDesc systemFontTextureDesc;
	memset(&systemFontTextureDesc, 0, sizeof(systemFontTextureDesc));
	systemFontTextureDesc.m_textureType = TextureType::Texture2D;
	systemFontTextureDesc.m_width = 512;
	systemFontTextureDesc.m_height = 512;
	systemFontTextureDesc.m_mipmapLevel = 0;
	systemFontTextureDesc.m_format = ImageFormat::R32;

	SubresourceDesc systemFontSubresourceDesc;
	memset(&systemFontSubresourceDesc, 0, sizeof(systemFontSubresourceDesc));
	systemFontSubresourceDesc.m_memory = tmpBitmap;
	systemFontSubresourceDesc.m_memoryPitch = 512;

	m_systemFontTexture = g_renderDevice->createTexture2D(systemFontTextureDesc, systemFontSubresourceDesc);

	delete[] tmpBitmap;
	delete[] ttfBuffer;

	initPrivate();
}

void FontManager::initPrivate()
{
	// create sampler state for font texture

	SamplerDesc fontTextureSamplerDesc;
	memset(&fontTextureSamplerDesc, 0, sizeof(fontTextureSamplerDesc));
	fontTextureSamplerDesc.m_minFilter = TextureFilter::Nearest;
	fontTextureSamplerDesc.m_magFilter = TextureFilter::Nearest;
	fontTextureSamplerDesc.m_wrapS = TextureWrap::ClampToEdge;
	fontTextureSamplerDesc.m_wrapT = TextureWrap::ClampToEdge;
	fontTextureSamplerDesc.m_anisotropyLevel = 1.0f;

	m_textureSampler = g_renderDevice->createSamplerState(fontTextureSamplerDesc);

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
	m_vertexBuffer = g_renderDevice->createBuffer(charBufferDesc, charSubresourceDesc);

	// create constant buffer
	BufferDesc constantBufferDesc;
	memset(&constantBufferDesc, 0, sizeof(constantBufferDesc));
	constantBufferDesc.m_bufferType = BufferType::ConstantBuffer;
	constantBufferDesc.m_bufferAccess = BufferAccess::Stream;
	constantBufferDesc.m_bufferMemorySize = sizeof(glm::mat4);

	SubresourceDesc constantSubresorceDesc;
	memset(&constantSubresorceDesc, 0, sizeof(constantSubresorceDesc));

	m_constantBuffer = g_renderDevice->createBuffer(constantBufferDesc, constantSubresorceDesc);

	// Create 2d font shader
	InputLayoutDesc layout[] =
	{
		{ "POSITION", 0, ImageFormat::RG32F,   0, (UINT)offsetof(FontVertex, position), INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, ImageFormat::RG32F,   0, (UINT)offsetof(FontVertex, texcoord),  INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, ImageFormat::RGBA32F,   0, (UINT)offsetof(FontVertex, color),  INPUT_PER_VERTEX_DATA, 0 },
	};

	m_shaderProgram = g_shaderManager->createShaderProgram("2d_font.vsh", "2d_font.psh", nullptr,
		layout, sizeof(layout) / sizeof(layout[0]));

	// create rasterizer state

	RasterizerStateDesc rasterizerState;
	memset(&rasterizerState, 0, sizeof(rasterizerState));
	rasterizerState.m_cullMode = CullMode::Back;
	rasterizerState.m_frontCCW = false;
	rasterizerState.m_fillMode = FillMode::Solid;

	m_rasterizerState = g_stateManager->createRasterizerState(rasterizerState);

	// create depth stencil test
	DepthStencilDesc desc = {};
	desc.m_depthEnable = false;
	desc.m_depthWriteMask = DEPTH_WRITE_MASK_ALL;
	desc.m_depthFunc = COMPARISON_ALWAYS;
	desc.m_stencilEnable = false;
	desc.m_frontFace.m_stencilFailOp = desc.m_frontFace.m_stencilDepthFailOp = desc.m_frontFace.m_stencilPassOp = STENCIL_OP_KEEP;
	desc.m_frontFace.m_stencilFunc = COMPARISON_ALWAYS;
	desc.m_backFace = desc.m_frontFace;
	m_depthStencilState = g_stateManager->createDepthStencilState(desc);
}

void FontManager::flushPrimitives()
{
	if (m_systemDrawStrings.empty())
		return;

	Assert2(g_renderer, "Called before renderer initialization.");
	Assert(m_systemFontTexture);
	Assert(m_vertexBuffer);

	// bind texture and their sampler
	g_renderDevice->setSampler(0, m_textureSampler);
	g_renderDevice->setTexture2D(0, m_systemFontTexture);

	// setup shader and others stuff
	g_shaderManager->setShaderProgram(m_shaderProgram);

	// build orthogonal matrix

	// get current view
	View* view = CameraProxy::getInstance()->getView();

	// calculate ortho matrix based on current view
	glm::mat4 proj = glm::ortho(0.0f, (float)view->m_width, 0.0f, (float)view->m_height, 0.1f, 100.0f);

	// update constant buffer
	void* constantBufferData = m_constantBuffer->map(BufferMapping::WriteOnly);
	memcpy(constantBufferData, &proj[0], sizeof(proj));
	m_constantBuffer->unmap();

	// setup it to shader
	g_renderDevice->setConstantBufferIndex(0, m_constantBuffer);

	// enable rasterizer state
	g_stateManager->setRasterizerState(m_rasterizerState);

	// disable depth test
	g_stateManager->setDepthStencilState(m_depthStencilState, 0);

	for (auto& it : m_systemDrawStrings)
	{
		// map our text buffer
		FontVertex* fontVertices = (FontVertex*)m_vertexBuffer->map(BufferMapping::WriteOnly);

		uint32_t numVertices = 0;
		size_t stringLength = it.m_string.length();

		for (int i = 0; i < stringLength; i++)
		{
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(m_systemFontChars, 512, 512, it.m_string[i] - 32, &it.m_x, &it.m_y, &q, 1);

			fontVertices[numVertices + 0].position = glm::vec2(q.x0, (float)view->m_height - q.y0); fontVertices[numVertices + 0].texcoord = glm::vec2(q.s0, q.t0);
			fontVertices[numVertices + 1].position = glm::vec2(q.x1, (float)view->m_height - q.y0); fontVertices[numVertices + 1].texcoord = glm::vec2(q.s1, q.t0);
			fontVertices[numVertices + 2].position = glm::vec2(q.x1, (float)view->m_height - q.y1); fontVertices[numVertices + 2].texcoord = glm::vec2(q.s1, q.t1);
			fontVertices[numVertices + 3].position = glm::vec2(q.x0, (float)view->m_height - q.y1); fontVertices[numVertices + 3].texcoord = glm::vec2(q.s0, q.t1);
			fontVertices[numVertices + 4].position = glm::vec2(q.x0, (float)view->m_height - q.y0); fontVertices[numVertices + 4].texcoord = glm::vec2(q.s0, q.t0);
			fontVertices[numVertices + 5].position = glm::vec2(q.x1, (float)view->m_height - q.y1); fontVertices[numVertices + 5].texcoord = glm::vec2(q.s1, q.t1);

			fontVertices[numVertices + 0].color = it.m_color;
			fontVertices[numVertices + 1].color = it.m_color;
			fontVertices[numVertices + 2].color = it.m_color;
			fontVertices[numVertices + 3].color = it.m_color;
			fontVertices[numVertices + 4].color = it.m_color;
			fontVertices[numVertices + 5].color = it.m_color;
			numVertices += 6;
		}

		// unmap
		m_vertexBuffer->unmap();

		// bind vertex buffer
		g_renderDevice->setVertexBuffer(m_vertexBuffer, sizeof(FontVertex), 0);

		// draw 
		g_renderDevice->draw(PM_TriangleList, 0, numVertices);
	}

	m_systemDrawStrings.clear();
}

void FontManager::shutdown()
{
	if (m_constantBuffer)
	{
		mem_delete(m_constantBuffer);
		m_constantBuffer = nullptr;
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

	if (m_systemFontTexture)
	{
		mem_delete(m_systemFontTexture);
		m_systemFontTexture = nullptr;
	}
}

void FontManager::drawSystemFont(const char* text, float x, float y, const glm::vec4& color)
{
	SystemStringDrawInfo drawInfo = {};
	drawInfo.m_string = text;
	drawInfo.m_x = x;
	drawInfo.m_y = y;
	drawInfo.m_color = color;
	m_systemDrawStrings.push_back(drawInfo);
}

}