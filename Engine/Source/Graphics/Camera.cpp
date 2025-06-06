#include "Camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Input/Mouse.h"
#include "Renderer.h"
#include "UniformBuffer.h"

const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 10000.0f;
const float SPEED = 50.0f;
const float FOLLOW_DISTANCE = 50.0f;

Camera::Camera(Renderer* renderer) :
	mCamConsts({glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f}),
	mView(glm::mat4(1.0f)),
	mProjection(glm::mat4(1.0f)),
	mTarget(glm::vec3(0.0f, 0.0f, 0.0f)),
	mForward(glm::normalize(mTarget - mCamConsts.position)),
	mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mRight(glm::normalize(glm::cross(mForward, mUp))),
	mPanDir(glm::vec3(0.0f, 0.0f, 0.0f)),
	mRenderer(renderer),
	mCameraBuffer(renderer->GetUniformBuffer("CameraBuffer")),
	mMode(CameraMode::Fly),
	mProjectionMode(GetProjectionFromRenderer(renderer)),
	mYaw(-90.0f),
	mPitch(0.0f),
	mRoll(0.0f),
	mFOV(45.0f),
	mAspectRatio(static_cast<float>(renderer->GetWidth()) / renderer->GetHeight()),
	mNearPlane(NEAR_PLANE),
	mFarPlane(FAR_PLANE),
	mFollowDistance(FOLLOW_DISTANCE)
{
	// Check to see if camera buffer was not loaded
	if (!mCameraBuffer)
	{
		// Create the camera buffer within renderer (ownership belongs in renderer)
		mCameraBuffer = renderer->CreateUniformBuffer(sizeof(CameraConsts), BufferBindingPoint::Camera, "CameraBuffer");
	}

	SetProjectionMode(mProjectionMode);
}

Camera::~Camera()
{
	std::cout << "Deleted camera" << std::endl;
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
		mProjection = glm::ortho(0.0f, static_cast<float>(mRenderer->GetWidth()), 0.0f, static_cast<float>(mRenderer->GetHeight()), mNearPlane, mFarPlane);
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

CameraProjection Camera::GetProjectionFromRenderer(Renderer* renderer)
{
	CameraProjection proj = {};
	switch (renderer->GetMode())
	{
	case RendererMode::MODE_2D:
		proj = CameraProjection::Orthographic;
		break;
	case RendererMode::MODE_3D:
		proj = CameraProjection::Perspective;
		break;
	}

	return proj;
}
