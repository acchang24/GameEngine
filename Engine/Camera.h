#pragma once
#include "glm/glm.hpp"

class Camera
{
public:
	Camera();
	~Camera();

	void SetActive();

	void SetPosition(const glm::vec3& pos) { mPosition = pos; }
	void SetTarget(const glm::vec3& target) { mTarget = target; }
	void SetYaw(float yaw) { mYaw = yaw; }
	void SetPitch(float pitch) { mPitch = pitch; }
	void SetRoll(float roll) { mRoll = roll; }

	float GetYaw() const { return mYaw; }
	float GetPitch() const { return mPitch; }

private:
	glm::vec3 mPosition;

	glm::vec3 mTarget;

	glm::vec3 mForward;

	glm::vec3 mUp;

	glm::vec3 mRight;

	float mYaw;
	float mPitch;
	float mRoll;

	float mFOV;
	float mNearPlane;
	float mFarPlane;

	bool mFirstMouse;
};