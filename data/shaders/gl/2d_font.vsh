#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec4 color;

out vec2 v_texcoord;
out vec4 v_color;

// Uniforms

layout(std140) uniform GlobalData
{
	mat4 u_modelViewProjection;
};

void main()
{
	v_texcoord = texcoord;
	v_color = color;
	gl_Position = u_modelViewProjection * vec4(position, 0.0, 1.0);
}
