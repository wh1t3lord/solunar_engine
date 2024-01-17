// RmlUI vertex shader

#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texcoord;

out vec2 v_texcoord;
out vec4 v_color;

layout(std140) uniform GlobalData
{
	vec4 u_translate;
	mat4 u_projectionMatrix;
};

void main()
{
	v_texcoord = texcoord;
	v_color = color;

	vec2 translatedPos = position + u_translate.xy;
	gl_Position = u_projectionMatrix * vec4(translatedPos, 0.0, 1.0);
}