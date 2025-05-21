#include "Camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Input/Mouse.h"
#include "Renderer3D.h"
#include "UniformBuffer.h"

const float FOV = 45.0f;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 10000.0f;
const float SPEED = 50.0f;
const float WIDTH_RATIO = 16.0f;
const float HEIGHT_RATIO = 9.0f;
const float FOLLOW_DISTANCE = 50.0f;
const float ORTHO_HEIGHT = 50.0f;

Camera::Camera() :
	mCamConsts({glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f}),
	mView(glm::mat4(1.0f)),
	mProjection(glm::mat4(1.0f)),
	mTarget(glm::vec3(0.0f, 0.0f, 0.0f)),
	mForward(glm::normalize(mTarget - mCamConsts.position)),
	mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mRight(glm::normalize(glm::cross(mForward, mUp))),
	mPanDir(glm::vec3(0.0f, 0.0f, 0.0f)),
	mCameraBuffer(Renderer3D::CreateUniformBuffer(sizeof(CameraConsts), BufferBindingPoint::Camera, "CameraBuffer")),
	mMode(CameraMode::First),
	mProjectionMode(CameraProjection::Perspective),
	mYaw(-90.0f),
	mPitch(0.0f),
	mRoll(0.0f),
	mFOV(FOV),
	mAspectRatio(WIDTH_RATIO / HEIGHT_RATIO),
	mNearPlane(NEAR_PLANE),
	mFarPlane(FAR_PLANE),
	mFollowDistance(FOLLOW_DISTANCE),
	mOrthoHeight(ORTHO_HEIGHT)
{
	SetProjectionMode(mProjectionMode);
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
		mForward = glm::normalize(CalculateRotation(1.0f, static_cast<float>(mYaw), static_cast<float>(mPitch)));

		mView = glm::lookAt(mCamConsts.position, mCamConsts.position + mForward, mUp);
		break;
	case CameraMode::Fly:
		mForward = glm::normalize(CalculateRotation(1.0f, static_cast<float>(mYaw), static_cast<float>(mPitch)));

		mView = glm::lookAt(mCamConsts.position, mCamConsts.position + mForward, mUp);
		break;
	case CameraMode::Orbit:
		mForward = glm::normalize(mTarget - mCamConsts.position);

		mView = glm::lookAt(mCamConsts.position, mTarget, mUp);
		break;
	case CameraMode::Third:
		mForward = glm::normalize(mTarget - mCamConsts.position);

		mView = glm::lookAt(mCamConsts.position, mTarget, mUp);
		break;
	}

	mRight = glm::normalize(glm::cross(mForward, mUp));

	mCamConsts.viewProjection = mProjection * mView;

	mCameraBuffer->UpdateBufferData(&mCamConsts);
}

void Camera::SetProjectionMode(CameraProjection mode)
{
	// Set the new mode
	mProjectionMode = mode;

	switch (mProjectionMode)
	{
	case CameraProjection::Perspective:
		mProjection = glm::perspective(glm::radians(mFOV), mAspectRatio, mNearPlane, mFarPlane);
		break;
	case CameraProjection::Orthographic:
		float orthoWidth = mOrthoHeight * mAspectRatio;
		mProjection = glm::ortho(-orthoWidth / 2.0f, orthoWidth / 2.0f, -mOrthoHeight / 2.0f, mOrthoHeight / 2.0f, mNearPlane, mFarPlane);
		break;
	}
}

void Camera::SetAspectRatio(float ratio)
{
	mAspectRatio = ratio;
	// Set the new projection with the new ratio
	SetProjectionMode(mProjectionMode);
}

void Camera::Update(float deltaTime, Mouse* mouse)
{
	double mouseX = mouse->GetX();
	double mouseY = mouse->GetY();

	Sint32 scrollDir = mouse->GetScrollDir();

	if (scrollDir >= 1)
	{
		mFollowDistance -= 1.0f;
		if (mFollowDistance <= 1.0f)
		{
			mFollowDistance = 1.0f;
		}
	}
	if (scrollDir <= -1)
	{
		mFollowDistance += 1.0f;
		if (mFollowDistance >= FOLLOW_DISTANCE + 100)
		{
			mFollowDistance = FOLLOW_DISTANCE + 100;
		}
	}

	// Update camera angles
	mYaw += mouseX;
	mPitch += mouseY;

	ClampPitch();

	if (mMode == CameraMode::Orbit)
	{
		// Offset from the target with yaw and pitch rotations
		glm::vec3 offset = CalculateRotation(mFollowDistance, static_cast<float>(mYaw), static_cast<float>(mPitch));

		SetPosition(mTarget + offset * -1.0f);
	}
	if (mMode == CameraMode::Third)
	{
		// Offset from the target with yaw and pitch rotations
		glm::vec3 offset = CalculateRotation(mFollowDistance, static_cast<float>(mYaw), static_cast<float>(mPitch));

		// Calculate new camera position behind the target
		SetPosition(mTarget - offset);
	}
	if (mMode == CameraMode::First || mMode == CameraMode::Fly)
	{
		SetPosition(GetPosition() + mPanDir * SPEED * deltaTime);
	}
}

void Camera::ToggleCameraModes()
{
	// Get the mode as int
	int mode = static_cast<int>(mMode);

	// Increment the mode (also loops back if end of modes)
	mode = (mode + 1) % static_cast<int>(CameraMode::Count);

	// Update the mode
	mMode = static_cast<CameraMode>(mode);
}

void Camera::ToggleProjectionMode()
{
	if (mProjectionMode == CameraProjection::Perspective)
	{
		SetProjectionMode(CameraProjection::Orthographic);
	}
	else
	{
		SetProjectionMode(CameraProjection::Perspective);
	}
}

glm::vec3 Camera::CalculateRotation(float distance, float yaw, float pitch) const
{
	return glm::vec3(
		distance * cosf(glm::radians(static_cast<float>(yaw))) * cosf(glm::radians(static_cast<float>(pitch))),
		distance * sinf(glm::radians(static_cast<float>(pitch))),
		distance * sinf(glm::radians(static_cast<float>(yaw))) * cosf(glm::radians(static_cast<float>(pitch)))
	);
}

void Camera::ClampPitch()
{
	if (mPitch >= 89.0)
	{
		mPitch = 89.0;
	}
	if (mPitch <= -89.0)
	{
		mPitch = -89.0;
	}
}
