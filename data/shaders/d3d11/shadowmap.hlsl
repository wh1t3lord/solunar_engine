// Shadow mapping shader
//

// C++ defines:
//	BILLBOARD
//	SKINNED

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
	float4 depthPosition: SV_Depth;
};

cbuffer GlobalData : register(b0)
{
	row_major float4x4 g_modelMatrix;
	row_major float4x4 g_viewMatrix;
	row_major float4x4 g_projectionMatrix;
	row_major float4x4 g_modelViewProjection;

	float4 g_viewPos;
	float4 g_viewDir;
};

cbuffer SkinningData : register(b1)
{
	row_major float4x4 g_bonesMatrices[64];
};

#ifndef SKINNED
VSOutput VSMain(VSInput input)
{
	VSOutput output = (VSOutput)0;

	// Position
	output.position = mul(float4(output.worldPos, 1.0f), g_viewMatrix);
	output.position = mul(output.position, g_projectionMatrix);	
	return output;
}
#else
VSOutput VSMain(VSInput input)
{
	VSOutput output = (VSOutput)0;

	// Position
	output.position = mul(float4(output.worldPos, 1.0f), g_viewMatrix);
	output.position = mul(output.position, g_projectionMatrix);	
	return output;
}
#endif