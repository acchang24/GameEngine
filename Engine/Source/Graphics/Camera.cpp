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
	mCameraBuffer(nullptr),
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

Camera::Camera(const glm::vec3& pos) :
	mCamConsts({ glm::mat4(1.0f), pos, 0.0f }),
	mView(glm::mat4(1.0f)),
	mProjection(glm::mat4(1.0f)),
	mTarget(glm::vec3(0.0f, 0.0f, 0.0f)),
	mForward(glm::normalize(mTarget - mCamConsts.position)),
	mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mRight(glm::normalize(glm::cross(mForward, mUp))),
	mPanDir(glm::vec3(0.0f, 0.0f, 0.0f)),
	mCameraBuffer(nullptr),
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
	std::cout << "Deleted camera" << std::endl;
}

void Camera::CreateBuffer(Renderer3D* renderer)
{
	if (mCameraBuffer == nullptr)
	{
		mCameraBuffer = renderer->CreateUniformBuffer(sizeof(CameraConsts), BufferBindingPoint::Camera, "CameraBuffer");
	}
}

void Camera::SetBuffer()
{
	// Update view matrix
	switch (mMode)
	{
	case CameraMode::First:
	case CameraMode::Fly:
		mView = glm::lookAt(mCamConsts.position, mCamConsts.position + mForward, mUp);
		break;
	case CameraMode::Orbit:
	case CameraMode::Third:
		mView = glm::lookAt(mCamConsts.position, mTarget, mUp);
		break;
	}

	// Calculate view * projection matrix to send to GPU
	mCamConsts.viewProjection = mProjection * mView;

	// Update buffer
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

	switch (mMode)
	{
	case CameraMode::First:
	case CameraMode::Fly:
		// Update position based on directional inputs
		mCamConsts.position = GetPosition() + mPanDir * SPEED * deltaTime;
		// Update camera's forward vector based on orientation
		mForward = glm::normalize(CalculateRotation(1.0f, static_cast<float>(mYaw), static_cast<float>(mPitch)));
		break;
	case CameraMode::Orbit:
		// Update camera position based on orientation
		mCamConsts.position = mTarget + (CalculateRotation(mFollowDistance, static_cast<float>(mYaw), static_cast<float>(mPitch)) * -1.0f);
		// Update camera's forward vector based on camera position and target position
		mForward = glm::normalize(mTarget - mCamConsts.position);
		break;
	case CameraMode::Third:
		// Update camera position based on orientation
		mCamConsts.position = mTarget - CalculateRotation(mFollowDistance, static_cast<float>(mYaw), static_cast<float>(mPitch));
		// Update camera's forward vector based on camera position and target position
		mForward = glm::normalize(mTarget - mCamConsts.position);
		break;
	}

	// Update the camera's right vector
	mRight = glm::normalize(glm::cross(mForward, mUp));
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
