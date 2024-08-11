#include "enginepch.h"
#include "engine/camera.h"
#include "engine/entity/entity.h"
#include "engine/entity/cameracomponent.h"

namespace solunar
{
	CameraProxy CameraProxy::ms_cameraProxy;

	void Camera::update()
	{
		m_view->m_view = getViewMatrix();
		m_view->updateInternalValues();

		glm::mat4 viewProjection = m_view->m_projection * m_view->m_view;
		m_frustum.update(viewProjection);
	}

	glm::vec3 Camera::getScreenRay(float x, float y)
	{
		if (getViewMatrix() != m_view->m_view)
			updateInternal();


		//glm::mat4 viewProjInverse = glm::inverse( m_view->m_projection  )

		return glm::vec3(1.0f);
	}

	void Camera::updateInternal()
	{
		m_view->m_view = getViewMatrix();
		m_view->updateInternalValues();

		glm::mat4 viewProjection = m_view->m_projection * m_view->m_view;
		m_frustum.update(viewProjection);
	}

	glm::mat4 Camera::getViewMatrix()
	{
		return glm::lookAt(m_position, m_position + m_direction, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void CameraProxy::updateProxy()
	{
		//	OPTICK_EVENT("CameraProxy::updateProxy");

		if (m_cameraComponent)
		{
			// update direction and position of camera based on component and node info
			m_direction = m_cameraComponent->getDirection();

			Assert(m_cameraComponent->getEntity());
			Entity* entity = m_cameraComponent->getEntity();

			m_position = entity->getWorldPosition();


			// update internal values
			updateInternal();
		}
	}

	void CameraProxy::setCameraComponent(CameraComponent* cameraComp)
	{
		Assert(cameraComp);
		m_cameraComponent = cameraComp;
	}

	CameraComponent* CameraProxy::getCameraComponent()
	{
		return m_cameraComponent;
	}

}