#include "Camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera() :
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mTarget(glm::vec3(0.0f, 0.0f, 0.0f)),
	mForward(glm::normalize(mTarget - mPosition)),
	mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mRight(glm::normalize(glm::cross(mUp, mForward))),
	mMode(CameraMode::Orbit),
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
	glm::mat4 view(1.0f);
	switch (mMode)
	{
	case CameraMode::First:
		mForward.x = cosf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
		mForward.y = sinf(glm::radians(static_cast<float>(mPitch)));
		mForward.z = sinf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
		// Normalize vector
		mForward = glm::normalize(mForward);

		// Re-calculate right vector
		mRight = glm::normalize(glm::cross(mForward, mUp));

		// Create view
		view = glm::lookAt(mPosition, mPosition + mForward, mUp);
		break;
	case CameraMode::Orbit:
		mForward.x = sinf(glm::radians(mYaw));
		mForward.y = sinf(glm::radians(mPitch));
		mForward.z = -cosf(glm::radians(mYaw)) * cosf(glm::radians(mPitch));
		// Normalize the vector
		mForward = glm::normalize(mForward);

		// Re-calculate right vector
		mRight = glm::normalize(glm::cross(mForward, mUp));

		// Create view
		view = glm::lookAt(mPosition, mTarget, mUp);
		break;
	}

	return view;
}
