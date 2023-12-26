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
	mMode(CameraMode::Fly),
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
		mForward.x = cosf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
		mForward.y = sinf(glm::radians(static_cast<float>(mPitch)));
		mForward.z = sinf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
		mForward = glm::normalize(mForward);

		mView = glm::lookAt(mPosition, mPosition + mForward, mUp);
		break;
	case CameraMode::Fly:
		mForward.x = cosf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
		mForward.y = sinf(glm::radians(static_cast<float>(mPitch)));
		mForward.z = sinf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
		mForward = glm::normalize(mForward);

		mView = glm::lookAt(mPosition, mPosition + mForward, mUp);
		break;
	case CameraMode::Orbit:
		mForward = glm::normalize(mTarget - mPosition);

		mView = glm::lookAt(mPosition, mTarget, mUp);
		break;
	}

	mRight = glm::normalize(glm::cross(mForward, mUp));
}
