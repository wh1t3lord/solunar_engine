// Common shader header
#ifndef COMMON_H
#define COMMON_H

// gamma correction
float4 gammaCorrection(float4 color)
{
	float gamma = 2.2;
	return float4(pow(color.rgb, vec3(gamma)), color.a);
}

float3 gammaCorrectionRGB(float3 color)
{
	float gamma = 2.2;
	return float3(pow(color.rgb, float3(gamma)));
}


#endif