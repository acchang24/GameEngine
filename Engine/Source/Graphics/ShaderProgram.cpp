#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include "../MemoryManager/AssetManager.h"
#include "../Util/LoggerMacros.h"

ShaderProgram::ShaderProgram(const std::string& shaderFile) :
    mType(LoadType(shaderFile)),
    mCode(ReadShaderFile(shaderFile.c_str())),
    mPath(shaderFile),
	mShaderID(CompileShader(mCode.c_str(), mType))
{
}

ShaderProgram::~ShaderProgram()
{
    LOG_DEBUG(std::string("Deleted shader program: \"" + mPath + "\" " + std::to_string(mShaderID)));
    
    std::cout << "Deleted shader program: \"" << mPath << "\" " << mShaderID << "\n";

	glDeleteShader(mShaderID);
}

GLenum ShaderProgram::LoadType(const std::string& shaderFile)
{
    std::string extension = shaderFile.substr(shaderFile.find_last_of('.'));

    GLenum type{};

    if (extension == ".vert")
    {
        type = GL_VERTEX_SHADER;
    }
    else if (extension == ".frag")
    {
        type = GL_FRAGMENT_SHADER;
    }
    else if (extension == ".geom")
    {
        type = GL_GEOMETRY_SHADER;
    }
    else if (extension == ".comp")
    {
        type = GL_COMPUTE_SHADER;
    }

    return type;
}

const std::string ShaderProgram::ReadShaderFile(const char* shaderFileName) const
{
    LOG_DEBUG("Loading shader file: " + std::string(shaderFileName));

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
        LOG_ERROR("Could not open the shader file: " + std::string(shaderFileName));
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

        std::string error = (typeString + " shader compilation failed: \"" + mPath + "\"\n" + infoLog);
        std::cout << error << "\n";
        LOG_ERROR(error);
    }

    return shader;
}
