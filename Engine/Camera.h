#pragma once
#include "glm/glm.hpp"

// Enum class for different types of camera modes
enum class CameraMode
{
	First,  // Used for first person
	Orbit,  // Used to orbit around a target object
	Fly     // Used for flying around the scene freely
};

// Camera class is mainly responsible for using the view matrix
// to simulate a camera. Contains an enum class CameraMode to 
// define what type of camera is being used. 
class Camera
{
public:
	Camera();
	~Camera();

	// SetActive is responsible for updating the camera's forward vector
	// as well as recalculating its right vector. It finally calculates the 
	// new view matrix.
	void SetActive();

	// Gets the camera's position
	// @return - const glm::vec3& for the camera's position
	const glm::vec3& GetPosition() const { return mPosition; }
	// Gets the camera's target position
	// @return - const glm::vec3& for the camera's target
	const glm::vec3& GetTarget() const { return mTarget; }
	// Gets the camera's forward vector
	// @returns - const glm::vec3& for the camera's forward
	const glm::vec3& GetForward() const { return mForward; }
	// Gets the camera's up vector
	// @return - const glm::vec3& for the camera's up vector
	const glm::vec3& GetUp() const { return mUp; }
	// Gets the camera's right vector
	// @return - const glm::vec3& for the camera's right vector
	const glm::vec3& GetRight() const { return mRight; }
	// Gets the camera's view matrix
	// @return - const mat4& for the camera's view matrix
	const glm::mat4& GetViewMatrix() const { return mView; }
	// Gets the camera's mode
	// @return - CameraMode for the camera's mode
	CameraMode GetCameraMode() const { return mMode; }

	// Sets the camera's position
	// @param - const glm::vec3& for the new position
	void SetPosition(const glm::vec3& pos) { mPosition = pos; }
	// Sets the camera's target position
	// @param - const glm::vec3& for the new target position
	void SetTarget(const glm::vec3& target) { mTarget = target; }

	double mYaw;
	double mPitch;
	double mRoll;

private:
	// Camera's position
	glm::vec3 mPosition;

	// Camera's target position
	glm::vec3 mTarget;

	// Camera's forward vector (normalized vector for the camera's facing direction)
	glm::vec3 mForward;

	// Camera's up vector (normalized vector pointing up)
	glm::vec3 mUp;

	// Camera's right vector (normalized vector pointing the camera's right)
	glm::vec3 mRight;

	// View matrix
	glm::mat4 mView;

	// Camera's current mode
	CameraMode mMode;

	float mFOV;
	float mNearPlane;
	float mFarPlane;
};