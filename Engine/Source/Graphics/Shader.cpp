#include "Shader.h"
#include <iostream>
#include <fstream>
#include "../MemoryManager/AssetManager.h"
#include "ShaderProgram.h"

Shader::Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile) :
	mShaderID(0)
{
    // Create a shader program and save the ID reference into mShaderID
    mShaderID = glCreateProgram();

    // Load and attach vertex shader
    ShaderProgram* vertexProgram = AssetManager::LoadShaderProgram(vertexFile);
    glAttachShader(mShaderID, vertexProgram->GetShaderID());

    // Load and attach fragment shader
    ShaderProgram* fragmentProgram = AssetManager::LoadShaderProgram(fragmentFile);
    glAttachShader(mShaderID, fragmentProgram->GetShaderID());

    // Load and attach geometry shader if it exists
    if (geometryFile != nullptr)
    {
        ShaderProgram* geometryProgram = AssetManager::LoadShaderProgram(geometryFile);
        glAttachShader(mShaderID, geometryProgram->GetShaderID());
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
        std::cout << "Shader program creation failed\n" << infoLog << "\n";
    }
}

Shader::~Shader()
{
    std::cout << "Deleted shader: " << mShaderID << "\n";
    glDeleteProgram(mShaderID);
    mShaderID = 0;
}
