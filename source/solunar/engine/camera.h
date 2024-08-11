#ifndef CAMERA_H
#define CAMERA_H

#include "graphics/view.h"
#include "graphics/camerafrustum.h"

namespace solunar
{
	class Camera
	{
	public:
		void  setView(View* view) { m_view = view; m_view->updateInternalValues(); }
		View* getView() { return m_view; }

		glm::vec3& getPosition() { return m_position; }

		void update();

		CameraFrustum& getFrustum() { return m_frustum; }

		glm::vec3 getScreenRay(float x, float y);

		void updateInternal();

		const glm::vec3 getDirection() const { return m_direction; }

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
		void updateProxy();

		void setCameraComponent(CameraComponent* cameraComp);
		CameraComponent* getCameraComponent();

	private:
		CameraComponent* m_cameraComponent;
	};
}

#endif // !CAMERA_H
