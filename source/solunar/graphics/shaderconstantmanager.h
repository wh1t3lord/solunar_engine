#ifndef SHADERCONSTANTMANAGER_H
#define SHADERCONSTANTMANAGER_H

#include "graphics/view.h"
#include "graphics/rendercontext.h"
//#include "graphics/light.h"
//#include "graphics/staticmeshcomponent.h"
#include "graphics/rendercontext.h"
#include "graphics/graphicsworld.h"

namespace engine
{
	enum class ShaderConstantType
	{
		Int,
		Float,
		Vector2,
		Vector3,
		Vector4,
		Matrix3x3,
//		Matrix3x4,
		Matrix4x4,
	};

	struct StaticMeshGlobalData
	{
		glm::mat4 m_modelMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_modelViewProjection;

		glm::vec4 m_viewPos;
		glm::vec4 m_viewDir;

		glm::vec4 m_directionLightVec;
		glm::vec4 m_directionLightColor;

		//int32_t m_lightsCount;

		//PointLightDesc m_pointLight;

		//PointLightDesc m_pointLights[MAX_POINT_LIGHTS];
	};

	class IShaderProgram;
	class StaticMeshComponent;

	class ShaderConstantManager : public Singleton<ShaderConstantManager>
	{
	public:
		ShaderConstantManager();
		~ShaderConstantManager();

		void init();
		void shutdown();

		void setStaticMeshGlobalData(StaticMeshComponent* meshComponent, View* view, RenderContext& renderContext, GraphicsWorld* graphicsWorld);

	//	void setPointLightConstantBuffer();
		
	//	void setDefaultContants(IShaderProgram* shader);
	//
	//	void setGraphicsConstants(IShaderProgram* shader);
	//	void setEngineConstants(IShaderProgram* shader);

	//	IBufferBase* getPointLightsConstantBuffer() { return m_pointLightsConstantBuffer; }

	private:
		IBufferBase* m_staticMeshConstantBuffer;

	//	IBufferBase* m_pointLightsConstantBuffer;
	};
}

#endif // !SHADERCONSTANTMANAGER_H
