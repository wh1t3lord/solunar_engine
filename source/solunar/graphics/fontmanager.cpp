#include "graphicspch.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "graphics/fontmanager.h"

#include "core/file/filesystem.h"
#include "core/file/contentdevice.h"

#include "graphics/core/device.h"
#include "graphics/core/texture.h"
#include "graphics/core/buffer.h"
#include "graphics/core/statemanager.h"

#include "graphics/vertexformat.h"

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
	stbtt_BakeFontBitmap(ttfBuffer, 0, 32.0f, tmpBitmap, 512, 512, 32, 96, m_systemFontChars);

	// flip buffer
	//Flip(tmpBitmap, 512 * 512, 512, 512);

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
	fontTextureSamplerDesc.m_minFilter = TextureFilter::Linear;
	fontTextureSamplerDesc.m_magFilter = TextureFilter::Linear;
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
	};

	m_shaderProgram = g_shaderManager->createShaderProgram("2d_font.vsh", "2d_font.psh", nullptr,
		layout, sizeof(layout) / sizeof(layout[0]));

	RasterizerStateDesc rasterizerState;
	memset(&rasterizerState, 0, sizeof(rasterizerState));
	rasterizerState.m_cullMode = CullMode::Back;
	rasterizerState.m_frontCCW = false;
	rasterizerState.m_fillMode = FillMode::Solid;

	m_rasterizerState = g_stateManager->createRasterizerState(rasterizerState);
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

void FontManager::drawSystemFont(const char* text, int x, int y)
{
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

	//// setup it to shader
	//m_defaultShaderProgram->setMatrix4(m_uOtrhoMatrixLocation, orthoMatrix);

	// map our text buffer
	FontVertex* fontVertices = (FontVertex*)m_vertexBuffer->map(BufferMapping::WriteOnly);

	uint32_t numVertices = 0;
	size_t stringLength = strlen(text);
	size_t offset = 0;

	for (int i = 0; i < stringLength; i++)
	{
		//if (text[i] >= 32 && text[i] < 128) {
			
			float fX = (float)x;
			float fY = (float)y;

			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(m_systemFontChars, 512, 512, text[i] - 32, &fX, &fY, &q, 1);

#if 1
			fontVertices[numVertices + 0].position = glm::vec2(q.x0 + offset, (float)view->m_height - q.y0); fontVertices[numVertices + 0].texcoord = glm::vec2(q.s0, q.t0);
			fontVertices[numVertices + 1].position = glm::vec2(q.x1 + offset, (float)view->m_height - q.y0); fontVertices[numVertices + 1].texcoord = glm::vec2(q.s1, q.t0);
			fontVertices[numVertices + 2].position = glm::vec2(q.x1 + offset, (float)view->m_height - q.y1); fontVertices[numVertices + 2].texcoord = glm::vec2(q.s1, q.t1);
			fontVertices[numVertices + 3].position = glm::vec2(q.x0 + offset, (float)view->m_height - q.y1); fontVertices[numVertices + 3].texcoord = glm::vec2(q.s0, q.t1);
			fontVertices[numVertices + 4].position = glm::vec2(q.x0 + offset, (float)view->m_height - q.y0); fontVertices[numVertices + 4].texcoord = glm::vec2(q.s0, q.t0);
			fontVertices[numVertices + 5].position = glm::vec2(q.x1 + offset, (float)view->m_height - q.y1); fontVertices[numVertices + 5].texcoord = glm::vec2(q.s1, q.t1);
#else
			fontVertices[numVertices + 0].position = glm::vec2(q.x0 + offset, -q.y1); fontVertices[numVertices + 0].texcoord = glm::vec2(q.s0, q.t0);
			fontVertices[numVertices + 1].position = glm::vec2(q.x1 + offset, -q.y1); fontVertices[numVertices + 1].texcoord = glm::vec2(q.s1, q.t0);
			fontVertices[numVertices + 2].position = glm::vec2(q.x1 + offset, -q.y0); fontVertices[numVertices + 2].texcoord = glm::vec2(q.s1, q.t1);
			fontVertices[numVertices + 3].position = glm::vec2(q.x0 + offset, -q.y0); fontVertices[numVertices + 3].texcoord = glm::vec2(q.s0, q.t1);
			fontVertices[numVertices + 4].position = glm::vec2(q.x0 + offset, -q.y1); fontVertices[numVertices + 4].texcoord = glm::vec2(q.s0, q.t0);
			fontVertices[numVertices + 5].position = glm::vec2(q.x1 + offset, -q.y0); fontVertices[numVertices + 5].texcoord = glm::vec2(q.s1, q.t1);
#endif

			offset += 20;

			numVertices += 6;
	/*	}*/
	}

	/*while (*text) {
		if (*text >= 32 && *text < 128) {
			stbtt_aligned_quad q;

			float fX = (float)x;
			float fY = (float)y;

			stbtt_GetBakedQuad(m_systemFontChars, 512, 512, *text - 32, &fX, &fY, &q, 1);

			fontVertices[0].position = glm::vec2(q.x0, q.y0); fontVertices[0].texcoord = glm::vec2(q.s0, q.t0);
			fontVertices[1].position = glm::vec2(q.x1, q.y0); fontVertices[1].texcoord = glm::vec2(q.s1, q.t0);
			fontVertices[2].position = glm::vec2(q.x1, q.y1); fontVertices[2].texcoord = glm::vec2(q.s1, q.t1);
			fontVertices[3].position = glm::vec2(q.x0, q.y1); fontVertices[3].texcoord = glm::vec2(q.s0, q.t1);
			fontVertices[4].position = glm::vec2(q.x0, q.y0); fontVertices[4].texcoord = glm::vec2(q.s0, q.t0);
			fontVertices[5].position = glm::vec2(q.x1, q.y1); fontVertices[5].texcoord = glm::vec2(q.s1, q.t1);

			fontVertices += 6;
			numVertices += 6;
		}
		++text;
	}*/


	// unmap
	m_vertexBuffer->unmap();

	// bind vertex buffer
	g_renderDevice->setVertexBuffer(m_vertexBuffer, sizeof(FontVertex), 0);

	// vertex format
	VertexFormat vf;
	vf.addTexcoord();
	vf.addTexcoord();
	g_renderDevice->setVertexFormat(&vf);

	// disable depth testing
	//glDisable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);

	// enable blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	g_stateManager->setRasterizerState(m_rasterizerState);

	// draw 
	//glDrawArrays(GL_TRIANGLES, 0, numVertices);
	g_renderDevice->draw(PM_TriangleList, 0, numVertices);

	// reset what we binded

	//glDisable(GL_BLEND);

	//glDepthMask(GL_TRUE);
	//glEnable(GL_DEPTH_TEST);

	//g_renderDevice->setTexture2D(0, nullptr);
	//g_renderDevice->setSampler(0, nullptr);
}

}