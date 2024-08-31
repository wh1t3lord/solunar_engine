#include "enginepch.h"
#include "engine/physics/physicsdebugdraw.h"
#include "graphics/debugrenderer.h"

namespace solunar {

PhysicsDebugDraw g_physicsDebugDraw;

void PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	glm::vec3 btFrom = glm::vec3(from.x(), from.y(), from.z());
	glm::vec3 btTo = glm::vec3(to.x(), to.y(), to.z());
	glm::vec3 btColor = glm::vec3(color.x(), color.y(), color.z());
	g_debugRender.DrawLine(btFrom, btTo, btColor);
}

void PhysicsDebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	btVector3 to = PointOnB + normalOnB * 1;//distance;
	const btVector3& from = PointOnB;
	drawLine(from, to, color);
}

void PhysicsDebugDraw::reportErrorWarning(const char* warningString)
{
}

void PhysicsDebugDraw::draw3dText(const btVector3& location, const char* textString)
{
}

void PhysicsDebugDraw::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

int PhysicsDebugDraw::getDebugMode() const
{
	return m_debugMode;
}

}