#version 330

uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = vec4(object_color, 1.f);
}