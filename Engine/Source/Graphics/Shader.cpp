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

    AssetManager* am = AssetManager::Get();

    // Load vertex shader
    ShaderProgram* vertexProgram = am->LoadShaderProgram(vertexFile);
    if (!vertexProgram)
    {
        vertexProgram = new ShaderProgram(vertexFile, GL_VERTEX_SHADER);
    }
    glAttachShader(mShaderID, vertexProgram->GetShaderID());

    // Load fragment shader
    ShaderProgram* fragmentProgram = am->LoadShaderProgram(fragmentFile);
    if (!fragmentProgram)
    {
        fragmentProgram = new ShaderProgram(fragmentFile, GL_FRAGMENT_SHADER);
    }
    glAttachShader(mShaderID, fragmentProgram->GetShaderID());

    // Load geometry shader if it exists
    if (geometryFile != nullptr)
    {
        ShaderProgram* geometryProgram = am->LoadShaderProgram(geometryFile);

        if (!geometryProgram)
        {
            geometryProgram = new ShaderProgram(geometryFile, GL_GEOMETRY_SHADER);
        }
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
    std::cout << "Deleted shader " << mShaderID << "\n";
    glDeleteProgram(mShaderID);
    mShaderID = 0;
}
