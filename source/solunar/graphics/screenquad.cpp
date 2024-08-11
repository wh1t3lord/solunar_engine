#include "graphicspch.h"
#include "graphics/screenquad.h"
#include "graphics/core/device.h"
#include "graphics/shaderprogram.h"
#include "graphics/ShaderProgramManager.h"
#include "graphics/renderer.h"

namespace solunar
{
	IBufferBase* ScreenQuad::ms_vertexBuffer;
	IShaderProgram* ScreenQuad::ms_screenQuadShader;
	InputLayoutDesc ScreenQuad::ms_inputLayout[2]=
	{
		{ "POSITION", 0, ImageFormat::RGBA32F,   0, (UINT)offsetof(QuadVertex, position), INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, ImageFormat::RG32F,   0, (UINT)offsetof(QuadVertex, texcoord),  INPUT_PER_VERTEX_DATA, 0 }
	};

	void ScreenQuad::init()
	{

		float quadVertices[] = {
			// positions         // texCoords
			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
			
			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
			 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,  1.0f, 1.0f
		};

		BufferDesc bufferDesc;
		memset(&bufferDesc, 0, sizeof(bufferDesc));
		bufferDesc.m_bufferType = BufferType::VertexBuffer;
		bufferDesc.m_bufferAccess = BufferAccess::Static;
		bufferDesc.m_bufferMemorySize = sizeof(quadVertices);
	
		SubresourceDesc subresourceDesc;
		memset(&subresourceDesc, 0, sizeof(subresourceDesc));
		subresourceDesc.m_memory = quadVertices;

		ms_vertexBuffer = g_renderDevice->createBuffer(bufferDesc, subresourceDesc);

		ms_screenQuadShader = g_shaderManager->createShaderProgram(
			"quad.vsh", 
			"quad.psh", 
			nullptr,
			ms_inputLayout,
			sizeof(ms_inputLayout) / sizeof(ms_inputLayout[0]));
	}

	void ScreenQuad::shutdown()
	{
		if (ms_vertexBuffer)
		{
			mem_delete(ms_vertexBuffer);
			ms_vertexBuffer = nullptr;
		}
	}

	void ScreenQuad::render(ITexture2D* texture)
	{
		Assert(texture);
		
		//glDisable(GL_DEPTH_TEST);

		g_renderDevice->setVertexBuffer(ms_vertexBuffer, sizeof(QuadVertex), 0);
		g_renderDevice->setTexture2D(0, texture);
		g_renderDevice->setSampler(0, g_defaultSampler);

		g_shaderManager->setShaderProgram(ms_screenQuadShader);
		g_renderDevice->draw(PM_TriangleList, 0, 6);
	}

	void ScreenQuad::render(ITexture2D* texture, IShaderProgram* shader)
	{
		Assert(texture);
		Assert(shader);
		
		//glDisable(GL_DEPTH_TEST);

		g_renderDevice->setVertexBuffer(ms_vertexBuffer, sizeof(QuadVertex), 0);
		g_renderDevice->setTexture2D(0, texture);

		g_shaderManager->setShaderProgram(shader);
		g_renderDevice->draw(PM_TriangleList, 0, 6);
	}

	void ScreenQuad::renderWithoutShaderBinding(ITexture2D* texture)
	{
		Assert(texture);

		/*glDisable(GL_DEPTH_TEST);*/

		g_renderDevice->setVertexBuffer(ms_vertexBuffer, sizeof(QuadVertex), 0);
		g_renderDevice->setTexture2D(0, texture);

		g_renderDevice->draw(PM_TriangleList, 0, 6);
	}

	void ScreenQuad::renderWithoutTextureBinding(IShaderProgram* shader)
	{
		Assert(shader);
		
		//glDisable(GL_DEPTH_TEST);

		g_renderDevice->setVertexBuffer(ms_vertexBuffer, sizeof(QuadVertex), 0);
		
		g_shaderManager->setShaderProgram(shader);

		g_renderDevice->draw(PM_TriangleList, 0, 6);
	}

	void ScreenQuad::renderWithoutShaderAndTextureBinding()
	{
		g_renderDevice->draw(PM_TriangleList, 0, 6);
	}
}