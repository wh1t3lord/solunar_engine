#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
#ifdef SKINNED
layout (location = 5) in vec4 weights;
layout (location = 6) in ivec4 boneIds;
#endif

out vec3 v_position;
out vec3 v_normal;
out vec2 v_texcoord;
out vec3 v_tangent;
out vec3 v_bitangent;

out mat3 v_tbn;

// #TODO: Remove to header

// This define must be has exactly size as MAX_POINT_LIGHTS in C++ code
#define MAX_POINT_LIGHTS 32

// This structure must be has exactly size as PointLightCB in C++ code
struct PointLight
{
	vec4 pos;

	// colors
	vec4 color;
	vec4 specular;

	// light data
	vec4 lightData;
};

// Uniforms

layout(std140) uniform GlobalData
{
	mat4 u_modelMatrix;
	mat4 u_viewMatrix;
	mat4 u_projectionMatrix;
	mat4 u_modelViewProjection;

	vec4 u_viewPos;
	vec4 u_viewDir;
	
	int u_lightsCount;
	
	PointLight u_pointLights[MAX_POINT_LIGHTS];
};

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