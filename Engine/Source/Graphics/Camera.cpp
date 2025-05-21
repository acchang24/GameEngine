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
const float SPEED = 100.0f;
const float WIDTH_RATIO = 16.0f;
const float HEIGHT_RATIO = 9.0f;
const float FOLLOW_DISTANCE = 25.0f;

static glm::mat4 s_Projection = glm::perspective(glm::radians(FOV), WIDTH_RATIO / HEIGHT_RATIO, NEAR_PLANE, FAR_PLANE);

Camera::Camera() :
	mCamConsts({glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f}),
	mView(glm::translate(glm::mat4(1.0f), mCamConsts.position)),
	mTarget(glm::vec3(0.0f, 0.0f, 0.0f)),
	mForward(glm::normalize(mTarget - mCamConsts.position)),
	mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mRight(glm::normalize(glm::cross(mForward, mUp))),
	mPanDir(glm::vec3(0.0f, 0.0f, 0.0f)),
	mCameraBuffer(Renderer3D::CreateUniformBuffer(sizeof(CameraConsts), BufferBindingPoint::Camera, "CameraBuffer")),
	mMode(CameraMode::First),
	mYaw(-90.0f),
	mPitch(0.0f),
	mRoll(0.0f),
	mFOV(FOV),
	mNearPlane(NEAR_PLANE),
	mFarPlane(FAR_PLANE),
	mFollowDistance(FOLLOW_DISTANCE)
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

	mCamConsts.viewProjection = s_Projection * mView;

	mCameraBuffer->UpdateBufferData(&mCamConsts);
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	return s_Projection;
}

void Camera::SetProjAspectRatio(float newAspectRatio)
{
	s_Projection = glm::perspective(FOV, newAspectRatio, NEAR_PLANE, FAR_PLANE);
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
		if (mFollowDistance >= 50.0f)
		{
			mFollowDistance = 50.0f;
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
