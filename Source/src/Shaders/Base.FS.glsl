#version 330

// Get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

void main()
{
	vec3 viewDir = normalize(world_normal);							// N
	vec3 lightDir = normalize(light_position - world_position);		// L
	vec3 V = normalize(eye_position - world_position);
	vec3 reflectDir = normalize(lightDir + V);						// H
	vec3 R = normalize(reflect(lightDir, world_normal));

	// Define ambient light component
	float ambient_light = 0.25f;

	// Compute diffuse light component
	float diffuse_light = material_kd * max(dot(viewDir, lightDir), 0.f);

	// Compute specular light component
	float specular_light = 0.f;

	if (diffuse_light > 0.f)
	{
		specular_light = material_ks * pow(max(dot(viewDir, reflectDir), 0.), material_shininess);
	}

	// Compute light
	float light = 0.f;
	
	float d	= distance(light_position, world_position);
	float attenuation_factor = 1.f / max(d, 0.f);
	light = ambient_light + (diffuse_light + specular_light);

	// Write pixel out color
	vec3 colour = object_color * light;

	out_color = vec4(colour, 1.f);
}