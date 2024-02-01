#ifndef DEBUGRENDERER_H
#define DEBUGRENDERER_H
#include "shaderprogram.h"
#include "graphics/vertexformat.h"
#include "core/math/boundingbox.h"

namespace engine {
	
class IShaderProgram;
class View;
class IBufferBase;

struct LineVertex
{
	glm::vec3 position;
	glm::vec3 color;
};

class DebugRender
{
public:
	DebugRender();
	~DebugRender();

	void initialize();
	void shutdown();

	void drawAxis(const glm::vec3& vec);
	void drawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color);
	void drawBoundingBox(const BoundingBox& box, const glm::vec3& color);
	void drawArc(const glm::vec3& center, const glm::vec3& normal, const glm::vec3& axis, float radiusA, float radiusB, float minAngle, float maxAngle,
		const glm::vec3& color, bool drawSect, float stepDegrees = float(10.f));

	void drawCone(float radius, float height, int upAxis, const glm::vec3& transform, const glm::vec3& color);
	

	void renderFrame(View* view);

	void pushModelMatrix(const glm::mat4& model);
	void popModelMatrix();

private:
	void beginDraw();
	void endDraw();

	void drawLinesInternal(View* view);

private:
	// Primitives
	struct Line
	{
		glm::vec3 from;
		glm::vec3 color0;

		glm::vec3 to;
		glm::vec3 color1;
	};

	std::vector<Line> m_lines;

	glm::mat4 m_modelMatrix;

private:
	VertexFormat m_lineVertexFormat;
	
	IBufferBase* m_verticesBuffer;
	IBufferBase* m_matrixBuffer;
	IShaderProgram* m_shaderProgram;
};

extern DebugRender g_debugRender;
	
}

#endif