#include "enginepch.h"
#include "engine/camera.h"
#include "engine/entity/entity.h"
#include "engine/entity/cameracomponent.h"

namespace solunar
{
	CameraProxy CameraProxy::ms_cameraProxy;

	void Camera::Update()
	{
		m_view->m_view = getViewMatrix();
		m_view->updateInternalValues();

		glm::mat4 viewProjection = m_view->m_projection * m_view->m_view;
		m_frustum.Update(viewProjection);
	}

	glm::vec3 Camera::GetScreenRay(float x, float y)
	{
		if (getViewMatrix() != m_view->m_view)
			UpdateInternal();


		//glm::mat4 viewProjInverse = glm::inverse( m_view->m_projection  )

		return glm::vec3(1.0f);
	}

	void Camera::UpdateInternal()
	{
		m_view->m_view = getViewMatrix();
		m_view->updateInternalValues();

		glm::mat4 viewProjection = m_view->m_projection * m_view->m_view;
		m_frustum.Update(viewProjection);
	}

	glm::mat4 Camera::getViewMatrix()
	{
		return glm::lookAt(m_position, m_position + m_direction, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void CameraProxy::UpdateProxy()
	{
		//	OPTICK_EVENT("CameraProxy::UpdateProxy");

		if (m_cameraComponent)
		{
			// update direction and position of camera based on component and node info
			m_direction = m_cameraComponent->GetDirection();

			Assert(m_cameraComponent->GetEntity());
			Entity* entity = m_cameraComponent->GetEntity();

			m_position = entity->GetPosition();


			// update internal values
			UpdateInternal();
		}
	}

	void CameraProxy::SetCameraComponent(CameraComponent* cameraComp)
	{
		Assert(cameraComp);
		m_cameraComponent = cameraComp;
	}

	CameraComponent* CameraProxy::GetCameraComponent()
	{
		return m_cameraComponent;
	}

}