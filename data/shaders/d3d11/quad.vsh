struct VS_OUTPUT
{
	float4 m_position : SV_POSITION;
	float2 m_texcoord : TEXCOORD;
};

VS_OUTPUT VSMain(float4 position : POSITION, float2 texcoord : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.m_position = position;
	output.m_texcoord = texcoord;
	return output;
}
