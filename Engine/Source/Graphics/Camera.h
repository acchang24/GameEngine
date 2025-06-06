#pragma once
#include <glm/glm.hpp>

// Enum class for different types of camera modes
enum class CameraMode
{
	First,  // Used for first person
	Orbit,  // Used to orbit around a target object
	Fly,    // Used for flying around the scene freely
	Third,  // Used for third person
	Count,   // Used for the number of modes
};

// Enum class for the type of projection the camera uses
enum class CameraProjection
{
	Perspective,
	Orthographic
};

// Struct for camera constants that are going to be sent to a buffer in the shaders
struct CameraConsts
{
	glm::mat4 viewProjection; // view * projection matrix
	glm::vec3 position;		  // camera position (view pos)
	float padding;			  // padding for allignment
};

class Mouse;
class Renderer;
class UniformBuffer;

// Camera class is mainly responsible for using the view matrix
// to simulate a camera. Contains an enum class CameraMode to 
// define what type of camera is being used. 
class Camera
{
public:
	Camera(Renderer* renderer);
	~Camera();

	// Updates the camera's view matrix, combines with the projection matrix 
	// and updates the CameraConsts struct to send to the shader's buffers
	void SetBuffer();

	// Gets the camera's constants
	// @return - const CameraConst& for the camera's constants
	const CameraConsts& GetCameraConsts() const { return mCamConsts; }

	// Gets the camera's view matrix
	// @return - const mat4& for the camera's view matrix
	const glm::mat4& GetViewMatrix() const { return mView; }

	// Gets the camera's projection matrix
	// @return - const mat4& for the camera's projection matrix
	const glm::mat4& GetProjectionMatrix() const { return mProjection; }

	// Gets the camera's position
	// @return - const glm::vec3& for the camera's position
	const glm::vec3& GetPosition() const { return mCamConsts.position; }

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

	// Gets the camera's yaw rotation
	// @return - double for the camera's yaw rotation
	double GetYaw() const { return mYaw; }

	// Gets the camera's pitch rotation
	// @return - double for the camera's pitch rotation
	double GetPitch() const { return mPitch; }

	// Gets the camera's roll rotation
	// @return - double for the camera's roll rotation
	double GetRoll() const { return mRoll; }

	// Gets the camera's FOV
	// @return - float for the camera's fov
	float GetFOV() const { return mFOV; }

	// Gets the camera's near plane
	// @return - float for the near plane
	float GetNearPlane() const {return mNearPlane; }

	// Gets the camera's far plane
	// @return - float for the far plane
	float GetFarPlane() const { return mFarPlane; }

	// Gets the camera's follow distance
	// @return - float for the distance
	float GetFollowDistance() const { return mFollowDistance; }


	// Sets the camera's position
	// @param - const glm::vec3& for the new position
	void SetPosition(const glm::vec3& pos) { mCamConsts.position = pos; }

	// Sets the camera's target position
	// @param - const glm::vec3& for the new target position
	void SetTarget(const glm::vec3& target) { mTarget = target; }

	// Sets the camera's pan direction
	// @param - const glm::vec3& for the direction
	void SetPanDir(const glm::vec3& dir) { mPanDir = dir; }

	// Sets the camera's mode
	// @param - CameraMode for the new mode
	void SetMode(CameraMode mode) { mMode = mode; }

	// Sets the camera's projection mode
	// @param - CameraProjection for the projection mode
	void SetProjectionMode(CameraProjection mode);

	// Sets the camera's yaw
	// @param - float for the new yaw rotation
	void SetYaw(float yaw) { mYaw = yaw; }

	// Sets the camera's pitch
	// @param - float for the new pitch rotation
	void SetPitch(float pitch) {  mPitch = pitch; }

	// Sets the camera's roll
	// @param - float for the new roll rotation
	void SetRoll(float roll) { mRoll = roll; }

	// Sets the camera's FOV
	// @param - float for the new fov
	void SetFOV(float fov) { mFOV = fov; }

	// Sets the camera's aspect ratio and updates the projection matrix with the new ratio
	// @param - float for the new ratio
	void SetAspectRatio(float ratio);

	// Sets the camera's follow distance
	// @param - float for the new distance
	void SetFollowDistance(float distance) { mFollowDistance = distance; }

	// Update the camera's pos and rotation
	// @param - float for the delta time
	// @param - Mouse* for the mouse
	void Update(float deltaTime, Mouse* mouse);

	// Toggles between all the camera modes
	void ToggleCameraModes();

	// Toggles between camera projection modes
	void ToggleProjectionMode();

private:
	// Calculates the camera's rotation and offset based on yaw and pitch rotations
	// @param - float distance for the offset
	// @param - float for yaw rotation
	// @param - float for pitch rotation
	glm::vec3 CalculateRotation(float distance, float yaw, float pitch) const;

	// Clamps the pitch rotation so that it doesn't go above 89 and below -89
	void ClampPitch();

	// Gets the projection type based on 2D or 3D renderer
	// @param - Renderer* for the renderer
	// @return - CameraProjection for projection type
	CameraProjection GetProjectionFromRenderer(Renderer* renderer);

	// Camera's constants
	CameraConsts mCamConsts;

	// View matrix
	glm::mat4 mView;

	// Projection matrix
	glm::mat4 mProjection;

	// Camera's target position (what the camera is looking at)
	glm::vec3 mTarget;

	// Camera's forward vector (normalized vector for the camera's facing direction)
	glm::vec3 mForward;

	// Camera's up vector (normalized vector pointing up)
	glm::vec3 mUp;

	// Camera's right vector (normalized vector pointing the camera's right)
	glm::vec3 mRight;

	// Camera's direction that it will pan/move to
	glm::vec3 mPanDir;

	// Pointer to the renderer
	Renderer* mRenderer;

	// Uniform buffer for camera (ownership of this taken care of in Renderer3D. Do not need to delete in destructor)
	UniformBuffer* mCameraBuffer;

	// Camera's current mode
	CameraMode mMode;

	// Camera's projection mode
	CameraProjection mProjectionMode;

	// Camera yaw
	double mYaw;

	// Camera pitch
	double mPitch;

	// Camera roll
	double mRoll;

	// Camera's field of view angle
	float mFOV;

	// Camera's aspect ratio
	float mAspectRatio;

	// Camera's near plane
	float mNearPlane;

	// Camera's far plane
	float mFarPlane;

	// Camera's follow distance (used for third person)
	float mFollowDistance;
};
