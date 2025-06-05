#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Keyframe for position
struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

// Keyframe for rotation
struct KeyRotation
{
	glm::quat rotation;
	float timeStamp;
};

// Keyframe for scale
struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

// Animation track for one bone
struct AnimationTrack
{
	std::vector<KeyPosition> positions;
	std::vector<KeyRotation> rotations;
	std::vector<KeyScale> scalings;
};

class Skeleton;

// Animation class responsible for loading and holding all the animation key frame data. A bone's
// keyframe data can be accessed by the bone's id or bone number. It will also interpolate the bone's
// position, rotation, and scaling matrices that can be applied to a bone at a specific time in the animation.
class Animation
{
public:
	// Animation constructor: Reads all the bone animation tracks and saves this animation to skeleton's vector of animations
	// @param - const aiAnimation* for the assimp animation
	// @param - const Skeleton* for the skeleton associated with this animation. This will add to skeleton's vector of animations
	// @param - const std::string& for the animation name
	Animation(const aiAnimation* anim, Skeleton* skeleton, const std::string& animName);
	~Animation();

	// Gets a bone's animation track by bone id/index. Returns nullptr if not found
	// @param - int for the bone id/index
	// @return - const AnimationTrack* for the bone's animation track, nullptr if not found
	const AnimationTrack* GetTrack(int boneID) const;

	// Interpolates a bone's position from the current key frame to the next
	// @param - float for the current time of the animation
	// @param - const AnimationTrack& for the bone being interpolated
	glm::mat4 InterpolatePosition(float currentTime, const AnimationTrack& track) const;

	// Interpolates a bone's rotation from the current key frame to the next
	// @param - float for the current time of the animation
	// @param - const AnimationTrack& for the bone being interpolated
	glm::mat4 InterpolateRotation(float currentTime, const AnimationTrack& track) const;

	// Interpolates a bone's scaling from the current key frame to the next
	// @param - float for the current time of the animation
	// @param - const AnimationTrack& for the bone being interpolated
	glm::mat4 InterpolateScaling(float currentTime, const AnimationTrack& track) const;

	// Gets the animation name
	// @return - const std::string& for the name
	const std::string& GetName() const { return mName; }

	// Gets the animation duration in ticks per second
	// @return - float for duration in ticks per second
	float GetDuration() const { return mDuration; }
	
	// Gets the ticks per second
	// @return - float for ticks per second
	float GetTicksPerSecond() const { return mTicksPerSecond; }

private:
	// Reads an animation's key frames, and saves each bone's track to the map of animation tracks
	// @param - const aiAnimation* for the assimp animation
	// @param - const Skeleton* for the skeleton associated with this animation. This will add to skeleton's vector of animations
	void ReadKeyFrames(const aiAnimation* anim, const Skeleton* skeleton);

	// Animation track associated with a skeleton's bone, by ID
	std::unordered_map<int, AnimationTrack> mAnimationTracks;

	// Animation name
	std::string mName;

	// Duration (in ticks per second)
	float mDuration;

	// Ticks per second
	float mTicksPerSecond;
};
