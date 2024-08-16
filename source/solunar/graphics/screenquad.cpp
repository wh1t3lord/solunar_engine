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

	void ScreenQuad::Init()
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

		ms_vertexBuffer = g_renderDevice->CreateBuffer(bufferDesc, subresourceDesc);

		ms_screenQuadShader = g_shaderManager->CreateShaderProgram(
			"quad.vsh", 
			"quad.psh", 
			nullptr,
			ms_inputLayout,
			sizeof(ms_inputLayout) / sizeof(ms_inputLayout[0]));
	}

	void ScreenQuad::Shutdown()
	{
		if (ms_vertexBuffer)
		{
			mem_delete(ms_vertexBuffer);
			ms_vertexBuffer = nullptr;
		}
	}

	void ScreenQuad::Render(ITexture2D* texture)
	{
		Assert(texture);
		
		//glDisable(GL_DEPTH_TEST);

		g_renderDevice->SetVertexBuffer(ms_vertexBuffer, sizeof(QuadVertex), 0);
		g_renderDevice->SetTexture2D(0, texture);
		g_renderDevice->SetSamplerState(0, g_defaultSampler);

		g_shaderManager->SetShaderProgram(ms_screenQuadShader);
		g_renderDevice->Draw(PM_TriangleList, 0, 6);
	}

	void ScreenQuad::Render(ITexture2D* texture, IShaderProgram* shader)
	{
		Assert(texture);
		Assert(shader);
		
		//glDisable(GL_DEPTH_TEST);

		g_renderDevice->SetVertexBuffer(ms_vertexBuffer, sizeof(QuadVertex), 0);
		g_renderDevice->SetTexture2D(0, texture);

		g_shaderManager->SetShaderProgram(shader);
		g_renderDevice->Draw(PM_TriangleList, 0, 6);
	}

	void ScreenQuad::RenderWithoutShaderBinding(ITexture2D* texture)
	{
		Assert(texture);

		/*glDisable(GL_DEPTH_TEST);*/

		g_renderDevice->SetVertexBuffer(ms_vertexBuffer, sizeof(QuadVertex), 0);
		g_renderDevice->SetTexture2D(0, texture);

		g_renderDevice->Draw(PM_TriangleList, 0, 6);
	}

	void ScreenQuad::RenderWithoutTextureBinding(IShaderProgram* shader)
	{
		Assert(shader);
		
		//glDisable(GL_DEPTH_TEST);

		g_renderDevice->SetVertexBuffer(ms_vertexBuffer, sizeof(QuadVertex), 0);
		
		g_shaderManager->SetShaderProgram(shader);

		g_renderDevice->Draw(PM_TriangleList, 0, 6);
	}

	void ScreenQuad::RenderWithoutShaderAndTextureBinding()
	{
		g_renderDevice->Draw(PM_TriangleList, 0, 6);
	}
}