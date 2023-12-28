#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Material.h"
#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"

// Shader class contains a OpenGL shader program that consists of
// a vertex shader and a fragment shader. This shader class manages
// when a particular shader program is being set as active, as well as
// functions to help set any uniforms set by its shaders.
class Shader
{
public:
	// Shader constructor takes the vertex and fragment shader files and
	// reads them into strings. It then compiles them into an OpenGL program
	// object, saving the reference into the mShaderID variable.
	// @param - const std::string& for the vertex shader name/file path
	// @param - const std::string& for the fragment shader name/file path
	Shader(const std::string& vertexFile, const std::string& fragmentFile);
	~Shader();

	// Reads a shader file's code line by line, saving it into a string.
	// @return - const std::string containing the shader code
	const std::string ReadShaderFile(const std::string& shaderFileName);

	// Compiles both the vertex and fragment shaders and links them into
	// a program object that can be referenced by mShaderID
    // @param - const char* for the vertex code
    // @param - const char* for the fragment code
	void CompileShaders(const char* vertexCode, const char* fragmentCode);

	// Sets this shader program as the active one with glUseProgram
	// Every shader/rendering call will use this program object and its shaders
	void SetActive() { glUseProgram(mShaderID); }

	// Getter for the shader program's id
	int GetID() const { return mShaderID; }

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

    // Sets a material uniform in a shader
    // @param - Material* to pass in the new material data
    void SetMaterial(Material* material) const
    {
        const MaterialColors& mats = material->GetMats();
        SetVec4("material.diffuseColor", mats.diffuseColor);
        SetVec4("material.specularColor", mats.specularColor);
        SetFloat("material.specularIntensity", mats.specularIntensity);
        SetFloat("material.shininess", mats.shininess);
        SetBool("material.hasDiffuseTexture", mats.hasDiffuseTexture);
        SetBool("material.hasSpecularTexture", mats.hasSpecularTexture);
        SetBool("material.hasEmissionTexture", mats.hasEmissionTexture);
    }

    void SetLight(const LightData& light) const
    {
        SetVec4("lightData.color", light.color);
        SetFloat("lightData.ambientIntensity", light.ambientIntensity);
        SetFloat("lightData.diffuseIntensity", light.diffuseIntensity);
        SetFloat("lightData.specularIntensity", light.specularIntensity);
        SetBool("lightData.isEnabled", light.isEnabled);
    }

    void SetPointLight(const PointLight* pointLight) const
    {
        std::string index = std::to_string(pointLight->mIndex);
        std::string name = "pointLights[" + index + "].data.color";
        SetVec4(name, pointLight->mData.color);
        name = "pointLights[" + index + "].data.ambientIntensity";
        SetFloat(name, pointLight->mData.ambientIntensity);
        name = "pointLights[" + index + "].data.diffuseIntensity";
        SetFloat(name, pointLight->mData.diffuseIntensity);
        name = "pointLights[" + index + "].data.specularIntensity";
        SetFloat(name, pointLight->mData.specularIntensity);
        name = "pointLights[" + index + "].data.isEnabled";
        SetBool(name, pointLight->mData.isEnabled);
        name = "pointLights[" + index + "].position";
        SetVec3(name, pointLight->mPointLightData.position);
        name = "pointLights[" + index + "].constant";
        SetFloat(name, pointLight->mPointLightData.constant);
        name = "pointLights[" + index + "].linear";
        SetFloat(name, pointLight->mPointLightData.linear);
        name = "pointLights[" + index + "].quadratic";
        SetFloat(name, pointLight->mPointLightData.quadratic);
    }

    void SetDirectionalLight(const DirectionalLight* directionalLight) const
    {
        std::string index = std::to_string(directionalLight->mIndex);
        std::string name = "directionalLights[" + index + "].data.color";
        SetVec4(name, directionalLight->mData.color);
        name = "directionalLights[" + index + "].data.ambientIntensity";
        SetFloat(name, directionalLight->mData.ambientIntensity);
        name = "directionalLights[" + index + "].data.diffuseIntensity";
        SetFloat(name, directionalLight->mData.diffuseIntensity);
        name = "directionalLights[" + index + "].data.specularIntensity";
        SetFloat(name, directionalLight->mData.specularIntensity);
        name = "directionalLights[" + index + "].data.isEnabled";
        SetBool(name, directionalLight->mData.isEnabled);
        name = "directionalLights[" + index + "].direction";
        SetVec3(name, directionalLight->mDirection);
    }

    void SetSpotLight(const SpotLight* spotlight) const
    {
        std::string index = std::to_string(spotlight->mIndex);
        std::string name = "spotlights[" + index + "].data.color";
        SetVec4(name, spotlight->mData.color);
        name = "spotlights[" + index + "].data.ambientIntensity";
        SetFloat(name, spotlight->mData.ambientIntensity);
        name = "spotlights[" + index + "].data.diffuseIntensity";
        SetFloat(name, spotlight->mData.diffuseIntensity);
        name = "spotlights[" + index + "].data.specularIntensity";
        SetFloat(name, spotlight->mData.specularIntensity);
        name = "spotlights[" + index + "].data.isEnabled";
        SetBool(name, spotlight->mData.isEnabled);
        name = "spotlights[" + index + "].position";
        SetVec3(name, spotlight->mSpotLightData.position);
        name = "spotlights[" + index + "].direction";
        SetVec3(name, spotlight->mSpotLightData.direction);
        name = "spotlights[" + index + "].cutoff";
        SetFloat(name, spotlight->mSpotLightData.cutoff);
        name = "spotlights[" + index + "].outerCutoff";
        SetFloat(name, spotlight->mSpotLightData.outerCutoff);
        name = "spotlights[" + index + "].constant";
        SetFloat(name, spotlight->mSpotLightData.constant);
        name = "spotlights[" + index + "].linear";
        SetFloat(name, spotlight->mSpotLightData.linear);
        name = "spotlights[" + index + "].quadratic";
        SetFloat(name, spotlight->mSpotLightData.quadratic);
    }

private:
	// The shader program object's reference ID
	unsigned int mShaderID;
};
