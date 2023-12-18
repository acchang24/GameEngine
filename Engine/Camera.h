#pragma once
#include "glm/glm.hpp"

class Camera
{
public:
	Camera();
	~Camera();

	glm::mat4 SetActive();

	void SetPosition(const glm::vec3& pos) { mPosition = pos; }
	void SetTarget(const glm::vec3& target) { mTarget = target; }

	double mYaw;
	double mPitch;
	double mRoll;

private:
	glm::vec3 mPosition;

	glm::vec3 mTarget;

	glm::vec3 mForward;

	glm::vec3 mUp;

	glm::vec3 mRight;

	float mFOV;
	float mNearPlane;
	float mFarPlane;

	bool mFirstMouse;
};