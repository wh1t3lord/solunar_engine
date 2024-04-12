#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "engine/entity/component.h"

namespace engine
{

class ENGINE_API CameraComponent : public Component
{
	ImplementObject(CameraComponent, Component);
public:
	CameraComponent();
	virtual ~CameraComponent();

	void setDirection(const glm::vec3& direction);
	glm::vec3 getDirection();

protected:
	glm::vec3 m_direction;
};


// Yaw pitch roll camera
class ENGINE_API CameraYawPitchRollComponent : public CameraComponent
{
	ImplementObject(CameraYawPitchRollComponent, CameraComponent);
public:
	CameraYawPitchRollComponent();
	virtual ~CameraYawPitchRollComponent();

	//! Update yaw, pitch and roll angle from mouse position.
	virtual void updateFromMousePosition(const glm::vec2 mousePos);

	//! Set directly your euler angles
	void setEulerRotation(float yaw, float pitch, float roll);

protected:
	float m_yaw;
	float m_pitch;
	float m_roll;
};

// First person camera
class ENGINE_API CameraFirstPersonComponent : public CameraYawPitchRollComponent
{
	ImplementObject(CameraFirstPersonComponent, CameraYawPitchRollComponent);
public:
	CameraFirstPersonComponent();
	virtual ~CameraFirstPersonComponent();

	//! Update yaw, pitch and roll angle from mouse position.
	virtual void updateFromMousePosition(const glm::vec2 mousePos);
};

}

#endif