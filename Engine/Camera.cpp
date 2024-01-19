#include "Camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Graphics/UniformBuffer.h"

Camera::Camera() :
	mCamConsts({glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f}),
	mView(glm::translate(glm::mat4(1.0f), mCamConsts.position)),
	mTarget(glm::vec3(0.0f, 0.0f, 0.0f)),
	mForward(glm::normalize(mTarget - mCamConsts.position)),
	mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mRight(glm::normalize(glm::cross(mUp, mForward))),
	mCameraBuffer(new UniformBuffer(sizeof(CameraConsts), BufferBindingPoint::Camera, "CameraBuffer")),
	mMode(CameraMode::Fly),
	mYaw(-90.0f),
	mPitch(0.0f),
	mRoll(0.0f),
	mFOV(45.0f),
	mNearPlane(0.1f),
	mFarPlane(10000.0f)
{

}

Camera::~Camera()
{
	std::cout << "Delete camera" << std::endl;

	delete mCameraBuffer;
}

void Camera::SetActive(const glm::mat4& proj)
{
	switch (mMode)
	{
	case CameraMode::First:
		mForward.x = cosf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
		mForward.y = sinf(glm::radians(static_cast<float>(mPitch)));
		mForward.z = sinf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
		mForward = glm::normalize(mForward);

		mView = glm::lookAt(mCamConsts.position, mCamConsts.position + mForward, mUp);
		break;
	case CameraMode::Fly:
		mForward.x = cosf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
		mForward.y = sinf(glm::radians(static_cast<float>(mPitch)));
		mForward.z = sinf(glm::radians(static_cast<float>(mYaw))) * cosf(glm::radians(static_cast<float>(mPitch)));
		mForward = glm::normalize(mForward);

		mView = glm::lookAt(mCamConsts.position, mCamConsts.position + mForward, mUp);
		break;
	case CameraMode::Orbit:
		mForward = glm::normalize(mTarget - mCamConsts.position);

		mView = glm::lookAt(mCamConsts.position, mTarget, mUp);
		break;
	}

	mRight = glm::normalize(glm::cross(mForward, mUp));

	mCamConsts.viewProjection = proj * mView;

	mCameraBuffer->UpdateBufferData(&mCamConsts);
}
