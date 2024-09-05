// Shadow mapping shader
//

// C++ defines:
//	BILLBOARD
//	SKINNED

#include "cbuffers.h"

struct VSInput
{
	float3 position		: POSITION;
#ifndef BILLBOARD
	float3 normal		: NORMAL;
#endif
	float2 texcoord		: TEXCOORD;
#ifndef BILLBOARD
	float3 tangent		: TANGENT;
	float3 bitangent	: BINORMAL;
#endif
#ifdef SKINNED
	float4 weights		: BLENDWEIGHT;
	float4 boneIds		: BLENDINDICES;
#endif
};

struct VSOutput
{
	float4 position		: SV_Position;
};

VSOutput VSMain(VSInput input)
{
	VSOutput output = (VSOutput)0;

#ifdef SKINNED
	// calculate bone transform
	row_major float4x4 skinMatrix = input.weights.x * g_bonesMatrices[int(input.boneIds.x)]
		+ input.weights.y * g_bonesMatrices[int(input.boneIds.y)]
		+ input.weights.z * g_bonesMatrices[int(input.boneIds.z)]
		+ input.weights.w * g_bonesMatrices[int(input.boneIds.w)];

	// World position
	output.position = mul(float4(input.position, 1.0f), skinMatrix);
	output.position = mul(output.position, g_modelMatrix);
#else
	// World position
	output.position = mul(float4(input.position, 1.0f), g_modelMatrix);
#endif

	output.position = mul(output.position, g_LightViewProjection);
	//output.position = mul(output.position, g_viewMatrix);	
	//output.position = mul(output.position, g_projectionMatrix);	
	return output;
}

void PSMain(VSOutput input)
{
}