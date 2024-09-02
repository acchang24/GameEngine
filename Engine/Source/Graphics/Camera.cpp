#include "Camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "UniformBuffer.h"

const float fov = 45.0f;
const float near = 0.1f;
const float far = 10000.0f;
const float panSpeed = 50.0f;

static glm::mat4 s_Projection = glm::perspective(fov, 16.0f / 9.0f, near, far);

Camera::Camera() :
	mCamConsts({glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f}),
	mView(glm::translate(glm::mat4(1.0f), mCamConsts.position)),
	mTarget(glm::vec3(0.0f, 0.0f, 0.0f)),
	mForward(glm::normalize(mTarget - mCamConsts.position)),
	mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mRight(glm::normalize(glm::cross(mUp, mForward))),
	mPanDir(glm::vec3(0.0f, 0.0f, 0.0f)),
	mCameraBuffer(new UniformBuffer(sizeof(CameraConsts), BufferBindingPoint::Camera, "CameraBuffer")),
	mMode(CameraMode::Fly),
	mYaw(-90.0f),
	mPitch(0.0f),
	mRoll(0.0f),
	mFOV(fov),
	mNearPlane(near),
	mFarPlane(far)
{

}

Camera::~Camera()
{
	std::cout << "Delete camera" << std::endl;

	delete mCameraBuffer;
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

	mCamConsts.viewProjection = s_Projection * mView;

	mCameraBuffer->UpdateBufferData(&mCamConsts);
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	return s_Projection;
}

void Camera::SetProjection(float newAspectRatio)
{
	glm::perspective(fov, newAspectRatio, near, far);
}

void Camera::Update(float deltaTime, double mouseX, double mouseY)
{
	// Update camera angles
	mYaw = mYaw + mouseX;
	mPitch = mPitch + mouseY;

	if (mPitch >= 89.0)
	{
		mPitch = 89.0;
		mouseY = 0.0f;
	}
	if (mPitch <= -89.0)
	{
		mPitch = -89.0;
		mouseY = 0.0f;
	}

	if (mMode == CameraMode::Orbit)
	{
		// Temp vec4 for camera position
		glm::vec4 pos(GetPosition(), 1.0f);
		// Temp vec4 for the camera's pivot point
		glm::vec4 pivot(mTarget, 1.0f);

		// Calculate rotation matrix along y axis (yaw)
		glm::mat4x4 rotationX(1.0f);
		// Rotate based on xOffset
		rotationX = glm::rotate(rotationX, glm::radians(static_cast<float>(-mouseX)), mUp);
		pos = (rotationX * (pos - pivot)) + pivot;

		// Calculate rotation matrix along x axis (pitch)
		glm::mat4x4 rotationY(1.0f);
		// Rotate based on yOffset
		glm::vec3 right = glm::normalize(glm::cross(mUp, glm::normalize(mTarget - glm::vec3(pos.x, pos.y, pos.z))));
		rotationY = glm::rotate(rotationY, glm::radians(static_cast<float>(-mouseY)), right);
		pos = (rotationY * (pos - pivot)) + pivot;

		SetPosition(glm::vec3(pos.x, pos.y, pos.z));
	}

	if (mMode == CameraMode::First || mMode == CameraMode::Fly)
	{
		mCamConsts.position = mCamConsts.position + mPanDir * panSpeed * deltaTime;
	}
}
