#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "engine/camera.h"

namespace engine {
	

class CameraManager : public Singleton<CameraManager>
{
private:
	static CameraManager ms_instance;
public:
	void setCamera(Camera* camera) { m_camera = camera; }
	Camera* getCamera() { return m_camera; }
	
	//void applyPostProcessEffect();
	//void applyCameraYawPitchRollEffect();
	
private:
	Camera* m_camera;
};
	
}

#endif