#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_texcoord;
out vec3 v_tangent;
out vec3 v_bitangent;

out mat3 v_tbn;

// shader uniforms
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

void main()
{
	// Position
	v_position = vec3(u_modelMatrix * vec4(position, 1.0));
	
	// texcoord
	v_texcoord = texcoord;
	
	// calculate normal matrix
	mat3 normalMatrix = transpose(inverse(mat3(u_modelMatrix)));
	
	// tangent & bitanget & normal
	v_tangent = normalMatrix * tangent;
	v_bitangent = normalMatrix * bitangent;
	v_normal = normalMatrix * normal;

	// removed tbn matrix building from fragment to vertex shader
	v_tbn = mat3(v_tangent, v_bitangent, v_normal);
	
	gl_Position = u_projectionMatrix * u_viewMatrix * vec4(v_position, 1.0);
}