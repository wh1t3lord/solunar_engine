#ifndef DEBUGRENDERER_H
#define DEBUGRENDERER_H

#include "shaderprogram.h"
#include "core/math/boundingbox.h"

namespace solunar {
	
class IShaderProgram;
class View;
struct IBufferBase;

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

	void Initialize();
	void Shutdown();

	void drawAxis(const glm::vec3& vec);
	void DrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color);
	void drawBoundingBox(const BoundingBox& box, const glm::vec3& color);
	void drawArc(const glm::vec3& center, const glm::vec3& normal, const glm::vec3& axis, float radiusA, float radiusB, float minAngle, float maxAngle,
		const glm::vec3& color, bool drawSect, float stepDegrees = float(10.f));

	void drawCone(float radius, float height, int upAxis, const glm::vec3& transform, const glm::vec3& color);
	void DrawPoly(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& color);

	void RenderFrame(View* view);

	void PushModelMatrix(const glm::mat4& model);
	void PopModelMatrix();

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
	IBufferBase* m_verticesBuffer;
	IBufferBase* m_matrixBuffer;
	IShaderProgram* m_shaderProgram;
};

extern DebugRender g_debugRender;
	
}

#endif