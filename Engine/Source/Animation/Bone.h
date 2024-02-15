#pragma once
#include <vector>
#include <string>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Struct for a key frame containing position information
struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

// Struct for a key frame containing rotation information
struct KeyRotation
{
	glm::quat rotation;
	float timeStamp;
};

// Struct for a key frame containing scaling information
struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

// Bones influence a vertex's position, rotation, and scaling. These bones are
// used within skeletal animation.
class Bone
{
public:
	// Bone constructor
	// @param - const std::string& for the bone's name
	// @param - int for the bone's id
	// @param - const glm::mat4& for the bone's offset matrix
	Bone(const std::string& name, int id, const glm::mat4& offset);
	~Bone();

	// Reads in all the position, rotation, and scale key frames for each bone
	// and saves it in a vector as well as its key frame count
	// @param - const aiNodeAnim* for the channel
	void ReadKeyFrames(const aiNodeAnim* channel);

	// Updates the bone's position, orientation (rotation), and scale matrices by interpolating 
	// between two keyframes. It uses those matrices to calculate a matrix to transform a bone into bone space
	// @param - float for the current time elapsed
	// @param - float for the animation's duration
	void Update(float animTime, float animDuration);

	// Interpolates the position based off the animation time and returns 
	// a translation matrix for the bone's position
	// @param - float for the current animation time
	// @param - float for the current animation's duration
	// @return - glm::mat4 for the bone's translation matrix
	glm::mat4 InterpolatePosition(float animTime, float animDuration) const;
	// Interpolates the rotation based off the animation time and returns 
	// a rotation matrix for the bone's rotation
	// @param - float for the current animation time
	// @return - glm::mat4 for the bone's rotation matrix
	glm::mat4 InterpolateRotation(float animTime, float animDuration) const;
	// Interpolates the scale based off the animation time and returns 
	// a scaling matrix for the bone's scale
	// @param - float for the current animation time
	// @return - glm::mat4 for the bone's scale matrix
	glm::mat4 InterpolateScaling(float animTime, float animDuration) const;

	// Gets the bone's local transform matrix (matrix to transform into bone space)
	// @return - const glm::mat4& for the bone's local transform matrix
	const glm::mat4& GetLocalTransform() const { return mLocalTransform; }

	// Gets the bone's offset matrix (matrix to transform bone into bind pose, or the inverse bind pose matrix)
	// @return - const glm::mat4& for the offset matrix
	const glm::mat4& GetOffetMatrix() const { return mOffset; }

	// Gets the bone's name
	// @return - const std::string& for the bone's name
	const std::string& GetBoneName() const { return mName; }

	// Gets the bone's id
	// @return - int for the bone's id
	int GetBoneID() const { return mID; }

private:
	// Array of the bone's positions in a key frame
	std::vector<KeyPosition> mPositions;
	// Array of the bone's rotations in a key frame
	std::vector<KeyRotation> mRotations;
	// Array of the bone's scalings in a key frame
	std::vector<KeyScale> mScalings;

	// Local transform matrix (matrix to transform the bone into bone space)
	glm::mat4 mLocalTransform;
	// Bone's offset matrix (matrix to transform bone into bind pose, or the inverse bind pose matrix)
	glm::mat4 mOffset;

	// The bone's name
	std::string mName;
	// The bone's id
	int mID;

	// Number of positions
	int mNumPositions;
	// Number of rotations
	int mNumRotations;
	// Number of scalings
	int mNumScalings;
};
