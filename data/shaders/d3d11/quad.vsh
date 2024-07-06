struct VS_OUTPUT
{
	float4 m_position : SV_POSITION;
	float2 m_texcoord : TEXCOORD;
};

VS_OUTPUT VSMain(float3 position : POSITION, float2 texcoord : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.m_position = float4(position, 1.0f);
	output.m_texcoord = texcoord;
	return output;
}
