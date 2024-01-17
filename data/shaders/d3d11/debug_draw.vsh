// Debug primitive drawing shader
//

struct VSInput
{
	float3 position : POSITION;
	float3 color : COLOR;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 color : COLOR;
};

cbuffer MatrixBuffer : register(b0)
{
	row_major float4x4 g_viewProjection;
};

VSOutput VSMain(VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(float4(input.position, 1.0f), g_viewProjection);
	output.color = input.color;
	return output;
}