#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 object_color;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;
out vec3 frag_color;

void main()
{
	// Compute world space vertex position and normal
	world_position = (Model * vec4(v_position, 1)).xyz;
	world_normal = normalize( mat3(Model) * normalize(v_normal));

	frag_color = object_color;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}