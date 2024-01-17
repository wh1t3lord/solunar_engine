// RmlUI vertex shader

struct VSInput
{
	float2 position : POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

cbuffer GlobalData : register(b0)
{
	float4 g_translate;
	row_major float4x4 g_projectionMatrix;
};

VSOutput VSMain(VSInput input)
{
	VSOutput output = (VSOutput)0;


	return output;
}
