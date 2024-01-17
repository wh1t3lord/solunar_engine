#version 330 core

layout(location=0) in vec2 position;
layout(location=1) in vec2 texcoord;

out vec2 v_texcoord;

uniform mat4 u_orthoMatrix;

void main()
{
	v_texcoord = texcoord;
	
	// Kirill: Another stupid hack
	vec2 hackedPosition = position;
	//hackedPosition.y = -hackedPosition.y;
	
	// Kirill: This is literally stupid hack
	vec2 posHomo = 	vec2(hackedPosition) - vec2(400, 300);
	posHomo /= vec2(400, 300);
	gl_Position = vec4(posHomo, 0, 1);
	
	//gl_Position = u_orthoMatrix * vec4(position, 0.0, 1.0);
	
	//gl_Position = vec4(position, 0.0, 1.0);
}
