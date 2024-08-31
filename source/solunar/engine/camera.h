#ifndef CAMERA_H
#define CAMERA_H

#include "graphics/view.h"
#include "graphics/camerafrustum.h"

namespace solunar
{
	class Camera
	{
	public:
		void  SetView(View* view) { m_view = view; m_view->updateInternalValues(); }
		View* GetView() { return m_view; }

		glm::vec3& GetPosition() { return m_position; }

		void Update();

		CameraFrustum& GetFrustum() { return m_frustum; }

		glm::vec3 GetScreenRay(float x, float y);

		void UpdateInternal();

		const glm::vec3 GetDirection() const { return m_direction; }

	private:
		glm::mat4 getViewMatrix();
		CameraFrustum m_frustum;

		View* m_view;

	public:
		glm::vec3 m_position = glm::vec3(0.0, 0.0, 1.0);
		glm::vec3 m_direction = glm::vec3(0.0, 0.0, 0.0);
		float yaw = 0.0f;
		float pitch = 0.0f;
		float roll = 0.0f;
	};

	class CameraComponent;

	class CameraProxy : public Singleton<CameraProxy>,
						public Camera
	{
	private:
		static CameraProxy ms_cameraProxy;

	public:
		void UpdateProxy();

		void SetCameraComponent(CameraComponent* cameraComp);
		CameraComponent* GetCameraComponent();

	private:
		CameraComponent* m_cameraComponent;
	};
}

#endif // !CAMERA_H
