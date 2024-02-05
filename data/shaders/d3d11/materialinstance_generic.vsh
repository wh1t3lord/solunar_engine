// Material Instance shader - Generic material
//

// C++ defines:
//	SKINNED

struct VSInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texcoord		: TEXCOORD;
	float3 tangent		: TANGENT;
	float3 bitangent	: BINORMAL;
#ifdef SKINNED
	float4 weights		: BLENDWEIGHT;
	uint boneIds		: BLENDINDICES;
#endif
};

struct VSOutput
{
	float4 position		: SV_Position;
	float3 worldPos		: TEXCOORD0;
	float3 normal		: NORMAL;
	float2 texcoord		: TEXCOORD1;
	float3 tangent		: TANGENT;
	float3 bitangent	: BINORMAL;
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

VSOutput VSMain(VSInput input)
{
	VSOutput output = (VSOutput)0;

	// World position
	output.worldPos = mul(input.position, g_modelMatrix);

	// Position
	output.position = mul(float4(output.worldPos, 1.0f), g_viewMatrix);
	output.position = mul(output.position, g_projectionMatrix);

	// texcoord
	output.texcoord = input.texcoord;

	// normal
	output.normal = mul( float4(input.normal, 1.0f), g_modelMatrix );
	
	return output;
}
