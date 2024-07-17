#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;
layout(location = 3) in vec4 color;

out vec2 out_texcoord;
out vec4 out_color;

// Uniforms

layout(std140) uniform GlobalData
{
	mat4 ProjectionMatrix;
};

void main()
{
	gl_Position = ProjectionMatrix * vec4(position, 0.0, 1.0);
	out_texcoord = texcoord;
	out_color = color;
}
