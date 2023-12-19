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
	mView(glm::translate(glm::mat4(1.0f), mPosition)),
	mMode(CameraMode::Orbit),
	mYaw(-90.0f),
	mPitch(0.0f),
	mRoll(0.0f),
	mFOV(45.0f),
	mNearPlane(0.1f),
	mFarPlane(100.0f)
{

}

Camera::~Camera()
{
	std::cout << "Delete camera" << std::endl;
}

void Camera::SetActive()
{
	switch (mMode)
	{
	case CameraMode::First:
		// Calculate the camera's forward based on yaw and pitch angles
		mForward.x = cosf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
		mForward.y = sinf(glm::radians(static_cast<float>(mPitch)));
		mForward.z = sinf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
		// Normalize vector
		mForward = glm::normalize(mForward);

		// Create view
		mView = glm::lookAt(mPosition, mPosition + mForward, mUp);
		break;
	case CameraMode::Fly:
		// Calculate the camera's forward based on yaw and pitch angles
		mForward.x = cosf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
		mForward.y = sinf(glm::radians(static_cast<float>(mPitch)));
		mForward.z = sinf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
		// Normalize vector
		mForward = glm::normalize(mForward);

		// Create view
		mView = glm::lookAt(mPosition, mPosition + mForward, mUp);
		break;
	case CameraMode::Orbit:
		// Calculate the camera's forward with the new position
		mForward = glm::normalize(mTarget - mPosition);

		// Create view
		mView = glm::lookAt(mPosition, mTarget, mUp);
		break;
	}

	// Re-calculate right vector
	mRight = glm::normalize(glm::cross(mForward, mUp));
}
