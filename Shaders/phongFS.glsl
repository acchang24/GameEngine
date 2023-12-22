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
    sampler2D specular0;
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

// Struct for point light position and attenuation terms
struct PointLight
{
    LightData data;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

// Struct for directional light
struct DirectionalLight
{
	LightData data;
	vec3 direction;
};

// Input variables from vertex shader call (same name and same type in vertex shader)
in vec3 normal;
in vec2 textureCoord;
in vec3 fragPos;

// Uniform for the 2D texture samplers
uniform TextureSamplers textureSamplers;
// Uniform for point light
uniform PointLight pointLight;
// Uniform for directional light
uniform DirectionalLight directionalLight;
// Uniform for Material
uniform Material material;
// uniform for view position (camera position)
uniform vec3 viewPos;

// Final vector4 fragment color output
out vec4 fragColor;

vec3 CalculatePhongLighting(LightData light, vec3 lightDir, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

void main() 
{
	vec3 lightResult = vec3(0.0, 0.0, 0.0);
	
	// Re-normalize the normal for dot product
	vec3 norm = normalize(normal);
	// Get the view direction from the fragment's position to the view(camera) position
	vec3 viewDir = normalize(viewPos - fragPos);

	if(pointLight.data.isEnabled)
	{
		lightResult += CalculatePointLight(pointLight, norm, viewDir, fragPos);
	}
	else
	{
		lightResult = pointLight.data.color.xyz * pointLight.data.ambientIntensity;
	}

	fragColor = vec4(lightResult, 1.0);

	// Add diffuse maps (textures)
    if(material.hasDiffuseTexture)
    {
		// Sampler colors of a texture with texture function, passing in sampler and coordinates
        vec4 textureColor = texture(textureSamplers.diffuse0, textureCoord);

        fragColor *= textureColor;
    }
}

vec3 CalculatePhongLighting(LightData light, vec3 lightDir, vec3 normal, vec3 viewDir)
{
	// Ambient light
	// Apply ambient intensity to the light's color to get ambient light
	vec3 ambientLight = light.color.xyz * light.ambientIntensity;

	// Diffuse light
	// Calculate diffuse impact of light on current fragment
	// with the dot product between the normal direction and the light direction vectors
	float diff = dot(normal, lightDir);
	// Use the max function to return highest value between the dot result earlier and 0.0f
	// to make sure the dot doesn't go negative when the angle between the two vectors
	// is greater than 90 degrees.
	diff = max(diff, 0.0);
	// Apply diffuse impact with the light's color to get diffuse light
	vec3 diffuseLight = diff * light.color.xyz;
	// Apply the material's diffuse color
	diffuseLight *= material.diffuseColor.xyz;
	// Apply light's diffuse intensity
	diffuseLight *= light.diffuseIntensity;

	// Specular light
	// Calculate reflect direction
	vec3 reflectDir = reflect(-lightDir, normal);
	// Calculate specular component
    // Calculate dot between view and reflect directions
    float spec = dot(viewDir, reflectDir);
    // Make sure it's not negative
    spec = max(spec, 0.0);
	// Raise to power of the material's specular intensity value (higher power = smaller, more focused highlight)
    spec = pow(spec, material.shininess);
	// Apply the material's specular intensity and spec component with light color to get specular light
	vec3 specularLight = material.specularIntensity * spec * light.color.xyz;
	// Apply the material's specular color
	specularLight *= material.specularColor.xyz;
	// Apply light's specular intensity
	specularLight *= light.specularIntensity;

	// Add specular maps
	if(material.hasSpecularTexture)
	{
		// Sampler colors of a specular map, passing in sampler and coordinates
		specularLight *= texture(textureSamplers.specular0, textureCoord).xyz;
	}

	// Combine three lights to get phong lighting
	vec3 phong = ambientLight + diffuseLight + specularLight;

	return phong;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	// Attenuation
	// Get the distance from fragment position to the point light's position
	float dist = length(pointLight.position - fragPos);
	// Calculate attentuation
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
	
	// Calculate the direction from fragment's position to the light source's position
	vec3 lightDir = normalize(light.position - fragPos);

	// Calculate phong lighting
	vec3 phong = CalculatePhongLighting(light.data, lightDir, normal, viewDir);

	// Add attenuation
	phong *= attenuation;

	return phong;
}