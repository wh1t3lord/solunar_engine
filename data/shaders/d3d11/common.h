// Common shader header
#ifndef COMMON_H
#define COMMON_H

// gamma correction
float3 gammaCorrectionRGB(float3 color)
{
	float gamma = 2.2;
	return float3(pow(color.rgb, float3(gamma, gamma, gamma)));
}

float3 pointLightPhong(float3 diffuseColor, float3 specularColor, float specularPower, float3 L, float3 N, float3 R)
{
	float diffuseLigthing = max(dot(N, L), 0);

	float specularStrength = 256.0f;
	float specularLigthing = pow(max(dot(R, L), 0.0), specularPower);
	
	return diffuseColor * diffuseLigthing + specularLigthing * specularColor * specularStrength;
}

float lightAttenuation(float3 lightPosition, float3 position, float radius)
{
	float3 distance = (lightPosition - position) / radius;
	float attenuation = 1.0f - dot(distance, distance);
	return attenuation;
}

float3 materialAlbedo(Texture2D texture2d, SamplerState samplerState, float2 texcoord)
{
	// sample texture
	float3 albedo = texture2d.Sample(samplerState, texcoord).rgb;
	
	// apply gamma correction and return the sampled albedo texture
	return gammaCorrectionRGB(albedo);
}

#endif