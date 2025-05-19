#pragma once
#include <glm/glm.hpp>

// Enum class for different types of camera modes
enum class CameraMode
{
	First,  // Used for first person
	Orbit,  // Used to orbit around a target object
	Fly     // Used for flying around the scene freely
};

// Struct for camera constants that are going to be sent to a buffer in the shaders
struct CameraConsts
{
	glm::mat4 viewProjection; // view * projection matrix
	glm::vec3 position;		  // camera position (view pos)
	float padding;			  // padding for allignment
};

class Mouse;
class UniformBuffer;

// Camera class is mainly responsible for using the view matrix
// to simulate a camera. Contains an enum class CameraMode to 
// define what type of camera is being used. 
class Camera
{
public:
	Camera();
	~Camera();

	// SetActive is responsible for updating the camera's forward vector
	// as well as recalculating its right vector. It then calculates the 
	// new view matrix. It combines the view matrix with the projection matrix 
	// and updates the CameraConsts struct to send to the shader's buffers
	void SetActive();

	// Gets the camera's constants
	// @return - const CameraConst& for the camera's constants
	const CameraConsts& GetCameraConsts() const { return mCamConsts; }
	// Gets the camera's position
	// @return - const glm::vec3& for the camera's position
	const glm::vec3& GetPosition() const { return mCamConsts.position; }
	// Gets the camera's view matrix
	// @return - const mat4& for the camera's view matrix
	const glm::mat4& GetViewMatrix() const { return mView; }
	// Gets the camera's projection matrix
	// @return - const mat4& for the camera's projection matrix
	const glm::mat4& GetProjectionMatrix() const;
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
	// Gets the camera's buffer
	UniformBuffer* GetCameraBuffer() { return mCameraBuffer; }
	// Gets the camera's mode
	// @return - CameraMode for the camera's mode
	CameraMode GetCameraMode() const { return mMode; }

	// Sets the camera's projection matrix
	// @param - float for the new aspect ratio
	static void SetProjection(float newAspectRatio);

	// Sets the camera's position
	// @param - const glm::vec3& for the new position
	void SetPosition(const glm::vec3& pos) { mCamConsts.position = pos; }
	// Sets the camera's target position
	// @param - const glm::vec3& for the new target position
	void SetTarget(const glm::vec3& target) { mTarget = target; }

	void SetPanDir(const glm::vec3& dir) { mPanDir = dir; }

	void Update(float deltaTime, Mouse* mouse);

private:
	// Camera's constants
	CameraConsts mCamConsts;

	// View matrix
	glm::mat4 mView;

	// Camera's target position
	glm::vec3 mTarget;

	// Camera's forward vector (normalized vector for the camera's facing direction)
	glm::vec3 mForward;

	// Camera's up vector (normalized vector pointing up)
	glm::vec3 mUp;

	// Camera's right vector (normalized vector pointing the camera's right)
	glm::vec3 mRight;

	// Camera's direction that it will pan/move to
	glm::vec3 mPanDir;

	// Uniform buffer for camera
	UniformBuffer* mCameraBuffer;

	// Camera's current mode
	CameraMode mMode;

	// Camera yaw
	double mYaw;

	// Camera pitch
	double mPitch;

	// Camera roll
	double mRoll;

	// Camera's field of view angle
	float mFOV;

	// Camera's near plane
	float mNearPlane;

	// Camera's far plane
	float mFarPlane;
};
