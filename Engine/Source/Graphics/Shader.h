#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

// Shader class contains a OpenGL program that attaches a vertex,
// fragment, geometry, shaders etc to a program. This shader class manages
// when a particular shader program is being set as active, as well as
// functions to help set any uniforms set by its shaders.
class Shader
{
public:
	// Shader constructor:
    // takes shader files and checks to see if they were already loaded in to 
    // the AssetManager. If not, it will create a ShaderProgram object based
    // on the shader file and its type. The ShaderProgram object will contain
    // a shader id that is used to attach to a program. After attaching all the 
    // shaders, this class will link the program and check if the program was
    // created successfully
	// object, saving the reference into the mShaderID variable.
	// @param - const char* for the vertex shader name/file path
	// @param - const char* for the fragment shader name/file path
    // @param - const char* for the geometry shader name/file path if it exists (defaults to nullptr)
	Shader(const std::string& name, const char* vertexFile, const char* fragmentFile, const char* geometryFile = nullptr);
	~Shader();

    // Finds all the uniform buffers used by this shader and calls glUniformBlockBinding on each one with this shader
    // to assign a binding point to the active uniform blocks
    void LinkShadersToUniformBlocks() const;

	// Sets this shader program as the active one with glUseProgram
	// Every shader/rendering call will use this program object and its shaders
	void SetActive() const { glUseProgram(mShaderID); }

	// Getter for the shader program's id
    // @return - unsigned int for the shader's id
	unsigned int GetID() const { return mShaderID; }

    // Sets bool uniform in a shader
    // @param - const std::string& for the uniform name
    // @param - bool for the new boolean value
    void SetBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(mShaderID, name.c_str()), static_cast<int>(value));
    }

    // Sets an int uniform in a shader
    // @param - const std::string& for the uniform name
    // @param - int for the new int value
    void SetInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(mShaderID, name.c_str()), value);
    }

    // Sets a float uniform in a shader
    // @param - const std::string& for the uniform name
    // @param - float for the new float value
    void SetFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(mShaderID, name.c_str()), value);
    }

    // Sets a vector2 uniform in a shader
    // @param - const std::string& for the uniform name
    // @param - const glm::vec2& for the new vector2
    void SetVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(mShaderID, name.c_str()), 1, &value[0]);
    }

    // Sets a vector3 uniform in a shader
    // @param - const std::string& for the uniform name
    // @param - const glm::vec3& for the new vector3
    void SetVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(mShaderID, name.c_str()), 1, &value[0]);
    }

    // Sets a vector4 uniform in a shader
    // @param - const std::string& for the uniform name
    // @param - const glm::vec4& for the new vector4
    void SetVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(mShaderID, name.c_str()), 1, &value[0]);
    }

    // Sets a matrix2 uniform in a shader
    // @param - const std::string& for the uniform name
    // @param - const glm::mat2& for the new matrix2
    void SetMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(mShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    // Sets a matrix3 uniform in a shader
    // @param - const std::string& for the uniform name
    // @param - const glm::mat3& for the new matrix3
    void SetMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(mShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    // Sets a matrix4 uniform in a shader
    // @param - const std::string& for the uniform name
    // @param - const glm::mat4& for the new matrix4
    void SetMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(mShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    std::string mName;

	// The shader program object's reference ID
	unsigned int mShaderID;
};
