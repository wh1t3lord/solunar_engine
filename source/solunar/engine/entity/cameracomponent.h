#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "engine/entity/component.h"

namespace solunar
{

class CameraComponent : public Component
{
	IMPLEMENT_OBJECT(CameraComponent, Component);
public:
	CameraComponent();
	virtual ~CameraComponent();

	void SetDirection(const glm::vec3& direction);
	glm::vec3 GetDirection();

protected:
	glm::vec3 m_direction;
};


// Yaw pitch roll camera
class CameraYawPitchRollComponent : public CameraComponent
{
	IMPLEMENT_OBJECT(CameraYawPitchRollComponent, CameraComponent);
public:
	CameraYawPitchRollComponent();
	virtual ~CameraYawPitchRollComponent();

	//! Update yaw, pitch and roll angle from mouse position.
	virtual void updateFromMousePosition(const glm::vec2& mousePos);

	//! Set directly your euler angles
	void SetEulerRotation(float yaw, float pitch, float roll);

	// #TODO: make getters 
public:
	float m_yaw;
	float m_pitch;
	float m_roll;
};

// First person camera
class CameraFirstPersonComponent : public CameraYawPitchRollComponent
{
	IMPLEMENT_OBJECT(CameraFirstPersonComponent, CameraYawPitchRollComponent);
public:
	CameraFirstPersonComponent();
	virtual ~CameraFirstPersonComponent();

	//! Update yaw, pitch and roll angle from mouse position.
	virtual void updateFromMousePosition(const glm::vec2& mousePos);
};

}

#endif