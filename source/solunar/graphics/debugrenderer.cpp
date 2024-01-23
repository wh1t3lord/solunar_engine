#include "graphicspch.h"
#include "graphics/core/device.h"
#include "graphics/debugrenderer.h"
#include "graphics/rendercontext.h"
#include "graphics/shaderprogram.h"
#include "graphics/ShaderProgramManager.h"
#include "graphics/view.h"

#define MAX_LINES_IN_VB 2048

namespace engine {

DebugRender g_debugRender;

DebugRender::DebugRender()
{
	m_verticesBuffer = nullptr;
	m_matrixBuffer = nullptr;
	m_shaderProgram = nullptr;
}

DebugRender::~DebugRender()
{
	m_verticesBuffer = nullptr;
	m_matrixBuffer = nullptr;
	m_shaderProgram = nullptr;
}

void DebugRender::initialize()
{
	// Vertex buffer

	BufferDesc linesBufferDesc;
	memset(&linesBufferDesc, 0, sizeof(linesBufferDesc));
	linesBufferDesc.m_bufferType = BufferType::VertexBuffer;
	linesBufferDesc.m_bufferAccess = BufferAccess::Dynamic; // D3D11_USAGE_DEFAULT and UpdateSubresource
	linesBufferDesc.m_bufferMemorySize = sizeof(Line) * MAX_LINES_IN_VB;

	SubresourceDesc linesSubresourceDesc;
	memset(&linesSubresourceDesc, 0, sizeof(linesSubresourceDesc));

	m_verticesBuffer = g_renderDevice->createBuffer(linesBufferDesc, linesSubresourceDesc);

	// Matrix buffer

	BufferDesc matrixBufferDesc;
	memset(&matrixBufferDesc, 0, sizeof(matrixBufferDesc));
	matrixBufferDesc.m_bufferType = BufferType::ConstantBuffer;
	matrixBufferDesc.m_bufferAccess = BufferAccess::Static; // D3D11_USAGE_DEFAULT and UpdateSubresource
	matrixBufferDesc.m_bufferMemorySize = sizeof(glm::mat4);

	SubresourceDesc matrixSubresourceDesc;
	memset(&matrixSubresourceDesc, 0, sizeof(matrixSubresourceDesc));

	m_matrixBuffer = g_renderDevice->createBuffer(matrixBufferDesc, matrixSubresourceDesc);

	// create shader
	m_shaderProgram = g_shaderManager->createShaderProgram("debug_draw.vsh", "debug_draw.psh");
	
	m_lineVertexFormat.addPosition(); // position
	m_lineVertexFormat.addPosition(); // color
}

void DebugRender::shutdown()
{
	//glDeleteVertexArrays(1, &m_vao);

	if (m_matrixBuffer)
	{
		mem_delete(m_matrixBuffer);
		m_matrixBuffer = nullptr;
	}

	if (m_verticesBuffer)
	{
		mem_delete(m_verticesBuffer);
		m_verticesBuffer = nullptr;
	}

	//g_varDebugDraw = nullptr;
}

void DebugRender::drawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color)
{
	Line line;
	line.from = from;
	line.color0 = color;
	line.to = to;
	line.color1 = color;
	m_lines.push_back(line);
}

void DebugRender::drawBoundingBox(const BoundingBox& box, const glm::vec3& color)
{
	glm::vec3 from = glm::vec3(box.m_min.x, box.m_min.y, box.m_min.z);
	glm::vec3 to = glm::vec3(box.m_max.x, box.m_max.y, box.m_max.z);

	glm::vec3 halfExtents = (to - from) * 0.5f;
	glm::vec3 center = (to + from) * 0.5f;
	int i, j;

	glm::vec3 edgecoord(1.f, 1.f, 1.f), pa, pb;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 3; j++)
		{
			pa = glm::vec3(edgecoord[0] * halfExtents[0], edgecoord[1] * halfExtents[1],
				edgecoord[2] * halfExtents[2]);
			pa += center;

			int othercoord = j % 3;
			edgecoord[othercoord] *= -1.f;
			pb = glm::vec3(edgecoord[0] * halfExtents[0], edgecoord[1] * halfExtents[1],
				edgecoord[2] * halfExtents[2]);
			pb += center;

			drawLine(pa, pb, color);
		}

		edgecoord = glm::vec3(-1.f, -1.f, -1.f);
		if (i < 3)
			edgecoord[i] *= -1.f;
	}
}

void DebugRender::renderFrame(View* view)
{
	beginDraw();

	// draw lines
	drawLinesInternal(view);

	// and clear them
	m_lines.clear();

	endDraw();
}

void DebugRender::drawDebugPositionOrNormal(const glm::vec3& vec)
{
	const float length = 0.2f;
	drawLine(vec, glm::vec3(vec.x + length, vec.y, vec.z), glm::vec3(1.0f, 0.0, 0.0f));
	drawLine(vec, glm::vec3(vec.x, vec.y + length, vec.z), glm::vec3(0.0f, 0.0f, 1.0f));
	drawLine(vec, glm::vec3(vec.x, vec.y, vec.z + length), glm::vec3(0.0f, 1.0f, 0.0f));
}

void DebugRender::beginDraw()
{
	//glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (int*)&m_savedVao);

	//glBindVertexArray(m_vao);
}

void DebugRender::endDraw()
{
	//glBindVertexArray(m_savedVao);
}

void updateMatrixBuffer(IBufferBase* buffer, const glm::mat4& viewProjection)
{
	glm::mat4* matrix = (glm::mat4*)buffer->map(BufferMapping::WriteOnly);
	memcpy(matrix, &viewProjection[0], sizeof(viewProjection));
	buffer->unmap();
}

void DebugRender::drawLinesInternal(View* view)
{
	if (m_lines.size() < 1)
		return;

	g_renderDevice->setVertexBuffer(m_verticesBuffer, sizeof(LineVertex), 0);
	g_renderDevice->setVertexFormat(&m_lineVertexFormat);
	
	//m_verticesBuffer->updateSubresource(m_lines.data(), m_lines.size() * sizeof(Line));

	LineVertex* vertices = (LineVertex*)m_verticesBuffer->map(BufferMapping::WriteOnly);
	memcpy(vertices, m_lines.data(), m_lines.size() * sizeof(Line));
	m_verticesBuffer->unmap();

	g_shaderManager->setShaderProgram(m_shaderProgram);

	glm::mat4 mVP = glm::mat4(1.0f);
	mVP = view->m_projection * view->m_view;

	//updateMatrixBuffer(m_matrixBuffer, mVP);
	m_matrixBuffer->updateSubresource(&mVP[0], sizeof(mVP));
	g_renderDevice->setConstantBufferIndex(0, m_matrixBuffer);

	g_renderDevice->draw(PM_LineList, 0, m_lines.size() * 2);
}

}