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

static glm::mat4 s_Projection = glm::perspective(FOV, WIDTH_RATIO / HEIGHT_RATIO, NEAR_PLANE, FAR_PLANE);

Camera::Camera() :
	mCamConsts({glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f}),
	mView(glm::translate(glm::mat4(1.0f), mCamConsts.position)),
	mTarget(glm::vec3(0.0f, 0.0f, 0.0f)),
	mForward(glm::normalize(mTarget - mCamConsts.position)),
	mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mRight(glm::normalize(glm::cross(mUp, mForward))),
	mPanDir(glm::vec3(0.0f, 0.0f, 0.0f)),
	mCameraBuffer(Renderer3D::CreateUniformBuffer(sizeof(CameraConsts), BufferBindingPoint::Camera, "CameraBuffer")),
	mMode(CameraMode::Fly),
	mYaw(-90.0f),
	mPitch(0.0f),
	mRoll(0.0f),
	mFOV(FOV),
	mNearPlane(NEAR_PLANE),
	mFarPlane(FAR_PLANE)
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

	mRight = glm::normalize(glm::cross(mUp, mForward));

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
		SetPosition(GetPosition() + mPanDir * SPEED * deltaTime);
	}
}
