#include "Camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera() :
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mTarget(glm::vec3(0.0f, 0.0f, 0.0f)),
	mForward(glm::vec3(0.0f, 0.0f, 0.0f)),
	mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
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
	std::cout << "Delete camera" << std::endl;
}

glm::mat4 Camera::SetActive()
{
	mForward.x = cosf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
	mForward.y = sinf(glm::radians(static_cast<float>(mPitch)));
	mForward.z = sinf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
	// Normalize vector
	mForward = glm::normalize(mForward);

	return glm::lookAt(mPosition, mPosition + mForward, mUp);
}
