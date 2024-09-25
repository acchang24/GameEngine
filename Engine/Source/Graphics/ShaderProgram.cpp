#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include "../MemoryManager/AssetManager.h"

ShaderProgram::ShaderProgram(const std::string& shaderFile, GLenum type) :
	mCode(ReadShaderFile(shaderFile.c_str())),
	mShaderID(CompileShader(mCode.c_str(), type))
{
    if (!mCode.empty() && mShaderID != 0)
    {
        AssetManager::Get()->SaveShaderProgram(shaderFile, this);
    }
}

ShaderProgram::~ShaderProgram()
{
    std::cout << "Deleted shader program " << mShaderID << "\n";
	glDeleteShader(mShaderID);
}

const std::string ShaderProgram::ReadShaderFile(const char* shaderFileName) const
{
	std::string shaderCode;

	std::ifstream shaderFile(shaderFileName);
	if (shaderFile.is_open())
	{
		std::string line;
		while (std::getline(shaderFile, line))
		{
			shaderCode += line + "\n";
		}
		shaderFile.close();
	}

    if (shaderCode.empty())
    {
        std::cout << "Could not open the shader file: " << shaderFileName << "\n";
    }
    
	return shaderCode;
}

unsigned int ShaderProgram::CompileShader(const char* shaderCode, GLenum type) const
{
    // Vertex shader
    // Create a vertex shader object with glCreateShader and store its id as an int
    unsigned int shader = glCreateShader(type);
    //   Attach shader code to the shader object:
    // - Takes the shader object to compile,
    // - How many strings that are going to be passed in as source code,
    // - The actual source code of the shader,
    // - An array of string lengths
    glShaderSource(shader, 1, &shaderCode, NULL);
    // Compile the shader
    glCompileShader(shader);

    // Check to see if shader compilation was successful:
    // Define an integer to indicate success
    int success = 0;
    // Char array for any error messages
    char infoLog[512];
    // Get any errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string typeString = "";
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        switch (type)
        {
        case GL_VERTEX_SHADER:
            typeString = "Vertex";
            break;
        case GL_FRAGMENT_SHADER:
            typeString = "Fragment";
            break;
        case GL_GEOMETRY_SHADER:
            typeString = "Geometry";
            break;
        }
        std::cout << typeString << " shader compilation failed\n" << infoLog << "\n";
    }

    return shader;
}
