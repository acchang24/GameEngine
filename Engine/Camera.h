#pragma once
#include "glm/glm.hpp"

// Enum class for different types of camera modes
enum class CameraMode
{
	First,
	Orbit
};

class Camera
{
public:
	Camera();
	~Camera();

	glm::mat4 SetActive();

	const glm::vec3& GetPosition() const { return mPosition; }
	const glm::vec3& GetTarget() const { return mTarget; }
	const glm::vec3& GetUp() const { return mUp; }

	CameraMode GetCameraMode() const { return mMode; }

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

	CameraMode mMode;

	float mFOV;
	float mNearPlane;
	float mFarPlane;

	bool mFirstMouse;
};