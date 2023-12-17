#include "Camera.h"

Camera::Camera() :
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mTarget(glm::vec3(0.0f, 0.0f, 0.0f)),
	mForward(glm::vec3(0.0f, 0.0f, 0.0f)),
	mUp(glm::vec3(0.0f, 0.0f, 0.0f)),
	mRight(glm::vec3(0.0f, 0.0f, 0.0f)),
	mYaw(-90.0f),
	mPitch(0.0f),
	mRoll(0.0f),
	mFOV(45.0f),
	mNearPlane(0.1f),
	mFarPlane(100.0f),
	mFirstMouse(true)
{

}

Camera::~Camera()
{

}

void Camera::SetActive()
{

}
