#include "Shader.h"
#include <iostream>
#include "../MemoryManager/AssetManager.h"
#include "../Util/Logger.h"
#include "Renderer.h"
#include "ShaderUniforms.h"
#include "ShaderProgram.h"

Shader::Shader(AssetManager* am, const std::string& name, const char* vertexFile, const char* fragmentFile, const char* geometryFile) :
    mName(name),
	mShaderID(0)
{
    // Create a shader program and save the ID reference into mShaderID
    mShaderID = glCreateProgram();

    // Load and attach vertex shader
    ShaderProgram* vertexProgram = am->LoadShaderProgram(vertexFile);
    glAttachShader(mShaderID, vertexProgram->GetShaderID());

    // Load and attach fragment shader
    ShaderProgram* fragmentProgram = am->LoadShaderProgram(fragmentFile);
    glAttachShader(mShaderID, fragmentProgram->GetShaderID());

    // Load and attach geometry shader if it exists
    if (geometryFile != nullptr)
    {
        ShaderProgram* geometryProgram = am->LoadShaderProgram(geometryFile);
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

        LOG_ERROR("Shader program creation failed\nOpenGL " + std::string(infoLog));
    }
    else
    {
        LinkShadersToUniformBlocks();
    }
}

Shader::~Shader()
{
    std::cout << "Deleted shader: " << mName << " "  << mShaderID << "\n";

    LOG_DEBUG("Deleted shader: " + mName + " " + std::to_string(mShaderID));

    glDeleteProgram(mShaderID);
    mShaderID = 0;
}

void Shader::LinkShadersToUniformBlocks() const
{
    GLint numBlocks = 0;
    // Gets the number of active uniform blocks used by this shader
    glGetProgramiv(mShaderID, GL_ACTIVE_UNIFORM_BLOCKS, &numBlocks);

    for (int i = 0; i < numBlocks; ++i)
    {
        GLint nameLen = 0;
        // Get the length of the uniform block name within the shader
        glGetActiveUniformBlockiv(mShaderID, i, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLen);

        // Get the name of the uniform block
        char* name = new char[nameLen];
        glGetActiveUniformBlockName(mShaderID, i, nameLen, NULL, name);
        std::string nameString = name;
        delete[] name;

        // Get the index of the uniform block
        int uniformBlockindex = glGetUniformBlockIndex(mShaderID, nameString.c_str());

        // Match the string block name to fixed layout slots
        int targetBindingPoint = GetBindingPointFromName(nameString);

        if (targetBindingPoint != -1)
        {
            // Assign a binding point to the active uniform block
            glUniformBlockBinding(mShaderID, uniformBlockindex, targetBindingPoint);
        }
    }
}

int Shader::GetBindingPointFromName(const std::string& blockName) const
{
    if (blockName == ShaderUniforms::CameraBuffer)
    {
        return static_cast<int>(BufferBindingPoint::Camera);
    }
    if (blockName == ShaderUniforms::LightBuffer)
    {
        return static_cast<int>(BufferBindingPoint::Lights);
    }
    if (blockName == ShaderUniforms::MaterialBuffer)
    {
        return static_cast<int>(BufferBindingPoint::Material);
    }
    if (blockName == ShaderUniforms::SkeletonBuffer)
    {
        return static_cast<int>(BufferBindingPoint::Skeleton);
    }
    if (blockName == ShaderUniforms::ShadowBuffer)  
    {
        return static_cast<int>(BufferBindingPoint::Shadow);
    }
    if (blockName == ShaderUniforms::PointShadowBuffer)
    {
        return static_cast<int>(BufferBindingPoint::PointShadow);
    }

    return -1;
}
