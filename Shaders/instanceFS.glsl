// Specify OpenGL 4.2 with core functionality
#version 420 core

#define MAX_LIGHTS 10
#define MAX_DIR_LIGHTS 1

// Struct to define a different number of texture units
// Add more samplers to this struct when needed
struct TextureSamplers
{
    sampler2D diffuse1;
    sampler2D specular1;
	sampler2D emission1;
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

// Struct for point light
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

// Struct for spotlight
struct SpotLight
{
    LightData data;
    vec3 position;
	float cutoff;
    vec3 direction;
    float outerCutoff;
    float constant;
    float linear;
    float quadratic;
};

// Input variables from vertex shader call (same name and same type in vertex shader)
in vec3 normal;
in vec2 textureCoord;
in vec3 fragPos;
in vec3 viewPosition;

// Uniform buffer for lights
layout (std140, binding = 1) uniform LightBuffer
{
	SpotLight spotlights[MAX_LIGHTS];
	PointLight pointLights[MAX_LIGHTS];
	DirectionalLight directionalLight[MAX_DIR_LIGHTS];
};

// Uniform buffer for materials
layout (std140, binding = 2) uniform MaterialBuffer
{
	vec4 diffuseColor;
	vec4 specularColor;
	float specularIntensity;
	float shininess;
	bool hasDiffuseTexture;
	bool hasSpecularTexture;
	bool hasEmissionTexture;
};

// Uniform for the 2D texture samplers
uniform TextureSamplers textureSamplers;

// Final vector4 fragment color output
out vec4 fragColor;

vec3 CalculatePhongLighting(LightData light, vec3 lightDir, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
vec3 CalculateDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

void main()
{
	vec3 lightResult = vec3(0.0, 0.0, 0.0);
	
	// Re-normalize the normal for dot product
	vec3 norm = normalize(normal);
	// Get the view direction from the fragment's position to the view(camera) position
	vec3 viewDir = normalize(viewPosition - fragPos);

	// Calculate directional light
	for(int i = 0; i < MAX_DIR_LIGHTS; ++i)
	{
		if(directionalLight[i].data.isEnabled)
		{
			lightResult += CalculateDirLight(directionalLight[i], norm, viewDir);
		}
	}
	
	for(int i = 0; i < MAX_LIGHTS; ++i)
	{
		// Calculate lighting from point lights
		if(pointLights[i].data.isEnabled)
		{
			lightResult += CalculatePointLight(pointLights[i], norm, viewDir, fragPos);
		}
		// Calculate lighting from spot lights
		if(spotlights[i].data.isEnabled)
		{
			lightResult += CalculateSpotLight(spotlights[i], norm, viewDir, fragPos);
		}
	}

	if(hasEmissionTexture)
	{
		vec3 emission = texture(textureSamplers.emission1, textureCoord).rgb;
		lightResult += emission;
	}

	fragColor = vec4(lightResult, 1.0);

	// Add diffuse maps (textures)
    if(hasDiffuseTexture)
    {
		// Sampler colors of a texture with texture function, passing in sampler and coordinates
        vec4 textureColor = texture(textureSamplers.diffuse1, textureCoord);

		if(textureColor.a < 0.1) 
		{
			discard;
		}

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
	diffuseLight *= diffuseColor.xyz;
	// Apply light's diffuse intensity
	diffuseLight *= light.diffuseIntensity;

	// Specular light

	//// Calculate reflect direction for phong
	//vec3 reflectDir = reflect(-lightDir, normal);
	//// Calculate dot between view and reflect directions for phong
	//float spec = dot(viewDir, reflectDir);

	// Halfway direction for blinn-phong
	vec3 halfwayDir = normalize(lightDir + viewDir);
	// dot between normal and halfwayDir for blinn-phong
	float spec = dot(normal, halfwayDir);

    // Make sure it's not negative
    spec = max(spec, 0.0);
	// Raise to power of the material's specular intensity value (higher power = smaller, more focused highlight)
    spec = pow(spec, shininess);
	// Apply the material's specular intensity and spec component with light color to get specular light
	vec3 specularLight = specularIntensity * spec * light.color.xyz;
	// Apply the material's specular color
	specularLight *= specularColor.xyz;
	// Apply light's specular intensity
	specularLight *= light.specularIntensity;

	// Add specular maps
	if(hasSpecularTexture)
	{
		// Sampler colors of a specular map, passing in sampler and coordinates
		specularLight *= texture(textureSamplers.specular1, textureCoord).xyz;
	}

	// Combine three lights to get phong lighting
	vec3 phong = ambientLight + diffuseLight + specularLight;

	return phong;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	// Attenuation
	// Get the distance from fragment position to the point light's position
	float dist = length(light.position - fragPos);
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

vec3 CalculateDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    // Calculate the direction from the fragment's position to the light source
    vec3 lightDir = normalize(-light.direction);

    // Calculate phong lighting
    vec3 phong = CalculatePhongLighting(light.data, lightDir, normal, viewDir);

    return phong;
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    // Calculate the direction from the fragment's position to the light source
    vec3 lightDir = normalize(light.position - fragPos);

    // Calculate theta, the angle between the vector from lightDir and the pointing direction of spotlight
    float theta = dot(lightDir, normalize(-light.direction));

    // Calculate epsilon, the cosine difference between the inner and outer cones
    float epsilon = light.cutoff - light.outerCutoff;

    // Calculate intensity for cone falloff
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    // Attenuation
    // Distance from fragment position to the point light's position
    float dist = length(light.position - fragPos);
    // Calculate attenuation
    float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    vec3 phong = CalculatePhongLighting(light.data, lightDir, normal, viewDir);

    phong *= attenuation * intensity;

    return phong;
}
