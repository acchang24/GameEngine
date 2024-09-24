#include "Shader.h"
#include <iostream>
#include <fstream>

Shader::Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile) :
	mShaderID(0)
{
    // Create a shader program and save the ID reference into mShaderID
    mShaderID = glCreateProgram();

    // Initialize shader object IDs
    unsigned int vertexShader = 0;
    unsigned int fragmentShader = 0;
    unsigned int geometryShader = 0;

    // Read and compile vertex shader
	std::string vertexCode = ReadShaderFile(vertexFile);
	if (vertexCode.empty())
	{
		std::cout << "Could not open the vertex shader file: " << vertexFile << "\n";
	}
    else
    {
        vertexShader = CompileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    }

    // Read and compile fragment shader
	std::string fragmentCode = ReadShaderFile(fragmentFile);
	if (fragmentCode.empty())
	{
		std::cout << "Could not open the fragment shader file: " << fragmentFile << "\n";
	}
    else
    {
        fragmentShader = CompileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
    }

    // Read and compile geometry shader if it exists
    if (geometryFile != nullptr)
    {
        std::string geometryCode = ReadShaderFile(geometryFile);
        if (geometryCode.empty())
        {
            std::cout << "Could not open the geometry shader file: " << geometryFile << "\n";
        }
        else
        {
            geometryShader = CompileShader(geometryCode.c_str(), GL_GEOMETRY_SHADER);
        }
    }

    // Link shader program
    glLinkProgram(mShaderID);

    // Check to see if program failed and retrieve the log
    int success = 0;
    char infoLog[512];
    glGetProgramiv(mShaderID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(mShaderID, 512, NULL, infoLog);
        std::cout << "Shader program creation failed\n" << infoLog << std::endl;
    }

    // Delete the shader objects once they have been linked to the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);
}

Shader::~Shader()
{
    std::cout << "Delete shader" << std::endl;
    glDeleteProgram(mShaderID);
    mShaderID = 0;
}

const std::string Shader::ReadShaderFile(const char* shaderFileName) const
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

	return shaderCode;
}

unsigned int Shader::CompileShader(const char* shaderCode, GLenum type) const
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
        std::cout << typeString << " shader compilation failed\n" << infoLog << std::endl;
    }

    // Attach the compiled shader to the program object
    glAttachShader(mShaderID, shader);

    return shader;
}
