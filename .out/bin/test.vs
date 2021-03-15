#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 Color;

uniform mat4 lookat;
uniform mat4 proj;

void main()
{
	Color = aColor;
	gl_Position = proj * lookat * vec4(aPos, 1.0f);
}