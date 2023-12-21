// Specify OpenGL 4.2 with core functionality
#version 420 core

// Struct to define material colors and properties of the surface
struct Material
{
    vec4 diffuseColor;
    vec4 specularColor;
	float specularIntensity;
    float shininess;
    bool hasDiffuseTexture;
    bool hasSpecularTexture;
};

// Struct to define a differnet number of texture units
// Add more samplers to this struct when needed
struct TextureSamplers
{
    sampler2D diffuse0;
    //sampler2D diffuse1;
    //sampler2D diffuse2;
    sampler2D specular0;
    //sampler2D specular1;
	//sampler2D specular2;
};

// Struct to define light data
struct LightData
{
	vec4 color;
	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;
	bool isEnabled;
};

// Input variables from vertex shader call (same name and same type in vertex shader)
in vec3 normal;
in vec2 textureCoord;
in vec3 fragPos;

// Uniform for the 2D texture samplers
uniform TextureSamplers textureSamplers;
// Uniform for LightData
uniform LightData lightData;
// Uniform for Material
uniform Material material;
// uniform for light position
uniform vec3 lightPos;
// uniform for view position (camera position)
uniform vec3 viewPos;

// Final vector4 fragment color output
out vec4 fragColor;

void main() 
{
	// Re-normalize the normal for dot product
	vec3 norm = normalize(normal);

	// Get the light direction from fragment's position to the light source
	vec3 lightDir = normalize(lightPos - fragPos);

	// Ambient light
	// Use light's ambient intensity
	float ambientStrength = lightData.ambientIntensity;
	// Apply ambient strength to the light's color to get ambient light
	vec4 ambientLight = ambientStrength * lightData.color;

	// Diffuse light
	// Calculate diffuse impact of light on current fragment
    // with the dot product between the normal direction and light direction vectors
	float diff = dot(norm, lightDir);
    // Use the max function to return highest between the dot result earlier and 0.0f 
    // to make sure the dot doesn't go negative when the angle between 
    // the two vectors is greater than 90 degrees
    diff = max(diff, 0.0);
	// Apply diffuse impact with the light's color to get diffuse light
	vec4 diffuseLight = diff * lightData.color;
	// Apply the material's diffuse color
	diffuseLight *= material.diffuseColor;
	// Apply light's diffuse intensity
	diffuseLight *= lightData.diffuseIntensity;

	// Specular light
	float specularStrength = material.specularIntensity;
	// Get the view direction (fragment's position to camera's position)
	vec3 viewDir = normalize(viewPos - fragPos);
	// Get the reflect direction
	vec3 reflectDir = reflect(-lightDir, norm);

	// Calculate specular component
    // Calculate dot between view and reflect directions
    float spec = dot(viewDir, reflectDir);
    // Make sure it's not negative
    spec = max(spec, 0.0);
    // Raise to power of the material's specular intensity value (higher power = smaller more focused highlight)
    spec = pow(spec, material.shininess);
	// Apply specular strength and spec component with light color to get specular light
	vec4 specularLight = specularStrength * spec * lightData.color;
	// Apply the material's specular color
	specularLight *= material.specularColor;
	// Apply light's specular intensity
	specularLight *= lightData.specularIntensity;

	// Add specular maps
	if(material.hasSpecularTexture)
	{
		specularLight *= texture(textureSamplers.specular0, textureCoord);
	}

	// Sampler colors of a texture with texture function, passing in sampler and coordinates
	fragColor = (ambientLight + diffuseLight + specularLight) * texture(textureSamplers.diffuse0, textureCoord);
}
