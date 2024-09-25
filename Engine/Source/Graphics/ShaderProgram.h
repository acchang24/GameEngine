#pragma once
#include <string>
#include <glad/glad.h>

// ShaderProgram describes an individual shader such as a vertex, fragment, geometry, etc. It is used
// to create a Shader object.This class will compile shader files and save its shader program as an unsigned int 
// for reference.Any ShaderProgram object is used within a Shader object that is used to render objects. 
// These ShaderProgram objects are typically de-allocated after attaching and successfully creating a Shader object
class ShaderProgram
{
public:
	// ShaderProgram constructor:
	// Reads and compiles a shader file. If successful, it
	// will save to the AssetManager's map of ShaderPrograms
	// @param - const char* for the shader's file name
	// @return - const std::string containing the shader code
	ShaderProgram(const std::string& shaderFile, GLenum type);
	~ShaderProgram();

	// Reads a shader file's code line by line, saving it into a string.
	// @param - const char* for the shader's file name
	// @return - const std::string containing the shader code
	const std::string ReadShaderFile(const char* shaderFileName) const;
	
	// Compiles a shader using a string that contains the shader code 
	// and links it into a program object that can be referenced by mShaderID
	// @param - const char* for the shader code
	// @param - GLenum for the shader type
	unsigned int CompileShader(const char* shaderCode, GLenum type) const;

	// Getter for the shader program's id
	// @return - unsigned int for the shader's id
	unsigned int GetShaderID() const { return mShaderID; }

private:
	// Shader program's code parsed and saved as a string
	std::string mCode;

	// Shader program's id
	unsigned int mShaderID;
};
