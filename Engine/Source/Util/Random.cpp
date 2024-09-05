#include "Random.h"

std::mt19937 Random::sGenerator;

void Random::Init()
{
	std::random_device rd;
	Random::Seed(rd());
}

void Random::Seed(unsigned int seed)
{
	sGenerator.seed(seed);
}

float Random::GetFloat()
{
	return GetFloatRange(0.0f, 1.0f);
}

float Random::GetFloatRange(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(sGenerator);
}

int Random::GetIntRange(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(sGenerator);
}

glm::vec2 Random::GetVector2(const glm::vec2& min, const glm::vec2& max)
{
	glm::vec2 r(GetFloat(), GetFloat());

	return min + (max - min) * r;
}

glm::vec3 Random::GetVector3(const glm::vec3& min, const glm::vec3& max)
{
	glm::vec3 r(GetFloat(), GetFloat(), GetFloat());

	return min + (max - min) * r;
}
