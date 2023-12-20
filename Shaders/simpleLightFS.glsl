// Specify OpenGL 4.2 with core functionality
#version 420 core

// Input variables from vertex shader call (same name and same type in vertex shader)
in vec3 normal;
in vec2 textureCoord;
in vec3 fragPos;

// Set a uniform for the 2D texture sampler
uniform sampler2D textureSampler;

// Uniform for light color
uniform vec4 lightColor;
// uniform for light position
uniform vec3 lightPos;

// Final vector4 fragment color output
out vec4 fragColor;

void main() 
{
	// Re-normalize the normal for dot product
	vec3 norm = normalize(normal);

	// Get the light direction from fragment's position to the light source
	vec3 lightDir = normalize(lightPos - fragPos);

	// Ambient light
	float ambientStrength = 0.1;
	// Apply ambient strength to the light's color to get ambient light
	vec4 ambientLight = ambientStrength * lightColor;

	// Diffuse light
	// Calculate diffuse impact of light on current fragment
    // with the dot product between the normal direction and light direction vectors
	float diff = dot(norm, lightDir);
    // Use the max function to return highest between the dot result earlier and 0.0f 
    // to make sure the dot doesn't go negative when the angle between 
    // the two vectors is greater than 90 degrees
    diff = max(diff, 0.0f);

	// Apply diffuse impact with the light's color to get diffuse light
    vec4 diffuseLight = diff * lightColor;

	// Sampler colors of a texture with texture function, passing in sampler and coordinates
	fragColor = (ambientLight + diffuseLight) * texture(textureSampler, textureCoord);
}