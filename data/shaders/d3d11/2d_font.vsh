// Font vertex shader

struct VSInput
{
	float2 texcoord : TEXCOORD;
	float2 position : POSITION;
	//float4 color : COLOR;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	//float4 color : COLOR;
};

cbuffer GlobalData : register(b0)
{
	row_major float4x4 g_modelViewProjection;
};

VSOutput VSMain(VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(g_modelViewProjection, float4(input.position, 0.0f, 1.0f));
	output.texcoord = input.texcoord;
	return output;
}
