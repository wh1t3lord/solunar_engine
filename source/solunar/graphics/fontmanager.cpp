#include "graphicspch.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "graphics/fontmanager.h"

#include "core/file/filesystem.h"
#include "core/file/contentdevice.h"


#include "graphics/core/device.h"
#include "graphics/core/texture.h"
#include "graphics/core/buffer.h"

#include "graphics/vertexformat.h"

// renderer
#include "graphics/view.h"
#include "graphics/renderer.h"

// Shader system
#include "graphics/shaderprogram.h"
#include "graphics/ShaderProgramManager.h"

#include <utility>

namespace engine
{

IFontManager* g_fontManager = nullptr;

// https://stackoverflow.com/questions/58905321/pointer-to-texture-data-flip-data-vertically
void FlipYTexture(const unsigned int width, const unsigned int height, uint8_t* data) {
	const unsigned int rowsSwapCount = height / 2;
	const unsigned int maxRowIndex = height - 1;

	for (unsigned int i = 0; i < rowsSwapCount; ++i) {
		for (unsigned int j = 0; j < width; ++j) {
			const unsigned int currentDataIndex = width * i + j;
			const unsigned int swapDataIndex = width * (maxRowIndex - i) + j;
			std::swap(data[currentDataIndex], data[swapDataIndex]);
		}
	}
}
void FontManager::initialize()
{
	Assert2(g_renderDevice, "Please initialize font manager after core graphics initialization");

	// load system font
	DataStreamPtr systemFontStream = g_contentManager->openStream("textures/ui/arial.ttf");
	systemFontStream->seek(Seek_End, 0);
	long systemFontFileLength = systemFontStream->tell();
	systemFontStream->seek(Seek_Begin, 0);

	uint8_t* ttfBuffer = new uint8_t[systemFontFileLength];
	systemFontStream->read(ttfBuffer, systemFontFileLength);

	uint8_t* tmpBitmap = new uint8_t[512 * 512]; //res of the bitmap
	stbtt_BakeFontBitmap(ttfBuffer, 0, 32.0f, tmpBitmap, 512, 512, 32, 96, m_systemFontChars);

#if 1
	// flip buffer
	//Flip(tmpBitmap, 512 * 512, 512, 512);

	FlipYTexture(512, 512, tmpBitmap);

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
#else
	unsigned char whiteTextureData[2][2] =
	{
		255, 255,
		255, 255
	};

	TextureDesc systemFontTextureDesc;
	memset(&systemFontTextureDesc, 0, sizeof(systemFontTextureDesc));
	systemFontTextureDesc.m_textureType = TextureType::Texture2D;
	systemFontTextureDesc.m_width = 2;
	systemFontTextureDesc.m_height = 2;
	systemFontTextureDesc.m_mipmapLevel = 0;
	systemFontTextureDesc.m_format = ImageFormat::R32;

	SubresourceDesc systemFontSubresourceDesc;
	memset(&systemFontSubresourceDesc, 0, sizeof(systemFontSubresourceDesc));
	systemFontSubresourceDesc.m_memory = whiteTextureData;

#endif
	m_systemFontTexture = g_renderDevice->createTexture2D(systemFontTextureDesc, systemFontSubresourceDesc);

	delete[] tmpBitmap;
	delete[] ttfBuffer;

	initPrivate();
}

struct FontVertex
{
	glm::vec2 position;
	glm::vec2 texcoord;
};

// Limited to 256 characters per one sentence
const int kMaxFontVBSize = sizeof(FontVertex) * 256;

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

	m_fontTextureSamplerState = g_renderDevice->createSamplerState(fontTextureSamplerDesc);

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
	m_fontVertexBuffer = g_renderDevice->createBuffer(charBufferDesc, charSubresourceDesc);

	// Create 2d font shader
	m_defaultShaderProgram = g_shaderManager->createShaderProgram("2d_font.vsh", "2d_font.psh", "#define INVTRANSPARENT\n");
}

void FontManager::shutdown()
{
	if (m_fontVertexBuffer)
	{
		mem_delete(m_fontVertexBuffer);
		m_fontVertexBuffer = nullptr;
	}

	if (m_fontTextureSamplerState)
	{
		mem_delete(m_fontTextureSamplerState);
		m_fontTextureSamplerState = nullptr;
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
	Assert(m_fontVertexBuffer);

	// bind texture and their sampler
	g_renderDevice->setSampler(0, m_fontTextureSamplerState);
	g_renderDevice->setTexture2D(0, m_systemFontTexture);

	// setup shader and others stuff
	g_shaderManager->setShaderProgram(m_defaultShaderProgram);


	//// build orthogonal matrix

	//// get current view
	//View* view = renderer->getView();

	//// calculate ortho matrix based on current view
	//glm::mat4 orthoMatrix = glm::identity<glm::mat4>();
	//orthoMatrix = glm::ortho(0, view->m_width, view->m_height, 0, -1, 1);

	//// setup it to shader
	//m_defaultShaderProgram->setMatrix4(m_uOtrhoMatrixLocation, orthoMatrix);

	// map our text buffer
	FontVertex* fontVertices = (FontVertex*)m_fontVertexBuffer->map(BufferMapping::WriteOnly);

	uint32_t numVertices = 0;
	size_t stringLength = strlen(text);
	size_t offset = 0;

	for (int i = 0; i < stringLength; i++)
	{
		//if (text[i] >= 32 && text[i] < 128) {
			
			float fX = (float)x;
			float fY = (float)y;

			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(m_systemFontChars, 512, 512, text[i] - 32, &fX, &fY, &q, 0);

#if 1
			fontVertices[numVertices + 0].position = glm::vec2(q.x0 + offset, q.y0); fontVertices[numVertices + 0].texcoord = glm::vec2(q.s0,  q.t0);
			fontVertices[numVertices + 1].position = glm::vec2(q.x1 + offset, q.y0); fontVertices[numVertices + 1].texcoord = glm::vec2(q.s1,  q.t0);
			fontVertices[numVertices + 2].position = glm::vec2(q.x1 + offset, q.y1); fontVertices[numVertices + 2].texcoord = glm::vec2(q.s1,  q.t1);
			fontVertices[numVertices + 3].position = glm::vec2(q.x0 + offset, q.y1); fontVertices[numVertices + 3].texcoord = glm::vec2(q.s0,  q.t1);
			fontVertices[numVertices + 4].position = glm::vec2(q.x0 + offset, q.y0); fontVertices[numVertices + 4].texcoord = glm::vec2(q.s0,  q.t0);
			fontVertices[numVertices + 5].position = glm::vec2(q.x1 + offset, q.y1); fontVertices[numVertices + 5].texcoord = glm::vec2(q.s1,  q.t1);
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
	m_fontVertexBuffer->unmap();

	// bind vertex buffer
	g_renderDevice->setVertexBuffer(m_fontVertexBuffer, sizeof(FontVertex), 0);

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

	// draw 
	//glDrawArrays(GL_TRIANGLES, 0, numVertices);

	// reset what we binded

	//glDisable(GL_BLEND);

	//glDepthMask(GL_TRUE);
	//glEnable(GL_DEPTH_TEST);

	g_renderDevice->setTexture2D(0, nullptr);
	g_renderDevice->setSampler(0, nullptr);
}

}