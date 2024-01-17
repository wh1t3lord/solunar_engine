#version 330 core

// copy of vertex layout from litgeneric
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

// shader output
out vec3 v_worldPos;

// shader uniforms
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix; 			// light view
uniform mat4 u_projectionMatrix; 	// light projection

void main()
{
	// translate vertex to world space
	vec3 worldPos = vec3( u_modelMatrix * vec4(position, 1.0) );
	v_worldPos = worldPos;
	
	// translate our world vertex to light space
	gl_Position = vec4( u_projectionMatrix * u_viewMatrix * vec4(worldPos, 1.0) );
}