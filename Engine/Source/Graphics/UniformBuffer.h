#pragma once
#include <glad/glad.h>

class Shader;

// Enum class for the different buffer binding points
enum class BufferBindingPoint
{
	Camera,
	Lights,
	Material,
	Skeleton,
	Shadow,
};

// UniformBuffer class helps abstract an OpenGL uniform buffer object. Use
// this class to help send the same data that may be sent to shaders multiple times.
class UniformBuffer
{
public:
	// UniformBuffer constructor generates the OpenGL uniform buffer object
	// @param - size_t for the buffer's size, or the amount of memory to allocate to the buffer
	// @param - BufferBindingPoint for the buffer's binding point
	// @param - const char* for the buffer's name
	UniformBuffer(size_t bufferSize, BufferBindingPoint bindingPoint, const char* bufferName);
	~UniformBuffer();

	// Gets the shader's uniform block index and links to the uniform buffer's binding point
	// @param - Shader* for the shader to link to
	void LinkShader(Shader* shader);

	// Updates the uniform buffer's data
	// @param - const void* for the new data
	void UpdateBufferData(const void* data);

private:
	// ID reference for the uniform buffer
	unsigned int mBufferID;

	// Size of the buffer
	unsigned int mBufferSize;

	// The uniform buffer's binding point
	unsigned int mBindingPoint;

	// The buffer's uniform block name in the shader
	const char* mBufferName;
};
