#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <assimp/scene.h>
#include <glm/glm.hpp>

// Struct for bone data
struct Bone
{
    glm::mat4 localTransform = glm::mat4(1.0f);         // Local transform relative to parent bone in original bind pose (t-pose). Used for chaining from parent/root bone
    glm::mat4 offsetMatrix = glm::mat4(1.0f);           // Transform from mesh space to bone space (inverse bind pose matrix)
    std::string name;                                   // Bone name
    int id;                                             // Bone id
    int parentID = -1;                                  // parent id (default to -1)
    bool influenceMesh = false;                         // If this bone influences any meshes or vertices (default false)
};

class Animation;

// Skeleton class is responsible for loading all the model's bone data and
// calculating the final bind pose at a specific time in an animation.
class Skeleton
{
public:
    // Skeleton constructor: Builds the skeleton's hierarchy and stores the bones in hierarchical order
    // in mBones. It will load all the data for a Bone in a model.
    // @param - const aiScene* for the model's scene
	Skeleton(const aiScene* scene);
	// Skeleton destructor:
    // The vector of Animations* is used purely for copying animations to a duplicate model. Models with the same
    // file will share the same skeleton. All ownership of any Animation objects will be handled by the AssetManager. 
    // Do not free/call delete on the Animations vector
    ~Skeleton();

    // Calculates and returns an array of matrices representing the final bone transformation matrices
    // for a pose at a given time in the animation.
    // @param - float for the time in the animation
    // @param - Animation* for the current animation
    // @return - const std::vector<glm::mat4> for the final bone matrix palette at a time
    const std::vector<glm::mat4> GetPoseAtTime(float animTime, const Animation* currAnim);

    const std::vector<glm::mat4> GetBindPose();

    // Gets the bone id from the bone map by name. Returns -1 if not found.
    // @param - const std::string& for the bone name
    // @return - int for the bone's id
    int GetBoneID(const std::string& name) const;

    // Adds an animation to the vector of animations
    void AddAnim(Animation* anim) { mAnimations.emplace_back(anim); }

    // Returns vector of animations. (Purely for copying repeated models)
    // @return - const std::vector<Animation*>& for the model's animations
    const std::vector<Animation*>& GetAnims() const { return mAnimations; }

private:
    // Build the bone hierarchy and store bone data into the vector of bones
    // @param - const aiNode* for the node that's being processed
    // @param - int for the parent id
    void BuildHierarchy(const aiNode* node, int parentID);

    // Loads in the inverse bind pose matrices, or offset matrix
    void LoadOffset(const aiScene* scene);

    // Map of bone name to index for quick access
    std::unordered_map<std::string, int> mBoneNameToID;

    // Vector of bones
    std::vector<Bone> mBones;

    // Vector of animations
    std::vector<Animation*> mAnimations;

    // Inverse of the root transform (need this to make models look normal scale)
    glm::mat4 mRootInverseTransform;
};
