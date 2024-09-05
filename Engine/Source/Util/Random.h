#pragma once
#include <random>
#include <glm/glm.hpp>

class Random
{
public:
	static void Init();

	// Seed the generator with a specified int
	// NOTE: Do not need to manually use this
	static void Seed(unsigned int seed);

	// @return - float between 0.0f and 1.0f
	static float GetFloat();

	// @param - float for the min value
	// @param - float for the max value
	// @return - float between the specified range
	static float GetFloatRange(float min, float max);

	// @param - int for the min value
	// @param - int for the max value
	// @return - int between the specified range
	static int GetIntRange(int min, int max);

	// @param - const glm::vec2& for the min vec2
	// @param - const glm::vec2& for the max vec2
	// @return - a random vector 2 given the min/max bounds
	static glm::vec2 GetVector2(const glm::vec2& min, const glm::vec2& max);

	// @param - const glm::vec3& for the min vec3
	// @param - const glm::vec3& for the max vec3
	// @return - a random vector 3 given the min/max bounds
	static glm::vec3 GetVector3(const glm::vec3& min, const glm::vec3& max);

private:
	static std::mt19937 sGenerator;
};