// Material Instance shader - Generic material
//

// C++ defines:
//	SKINNED (Vertex Factory)
//	UNLIT (Pixel Variation)

#include "common.h"
#include "cbuffers.h"

struct VSInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texcoord		: TEXCOORD;
	float3 tangent		: TANGENT;
	float3 bitangent	: BINORMAL;
#ifdef SKINNED
	float4 weights		: BLENDWEIGHT;
	float4 boneIds		: BLENDINDICES;
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

Texture2D 			g_albedoTexture : register(t0);
SamplerState		g_albedoSampler : register(s0);
Texture2D 			g_normalTexture : register(t1);
SamplerState		g_normalSampler : register(s1);

// Vertex shader
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
	output.worldPos = mul(float4(input.position, 1.0f), skinMatrix);
	output.worldPos = mul(float4(output.worldPos, 1.0f), g_modelMatrix);
#else
	// World position
	output.worldPos = mul(float4(input.position, 1.0f), g_modelMatrix);
#endif

	// #TODO: multiply output.worldPos by g_viewProjection matrix.

	// Position
	output.position = mul(float4(output.worldPos, 1.0f), g_viewMatrix);
	output.position = mul(output.position, g_projectionMatrix);

	// texcoord
	output.texcoord = input.texcoord;

#ifdef SKINNED
	// normal
	output.normal = normalize(mul(float4(input.normal, 0.0f), skinMatrix));
	output.normal = mul(float4(output.normal, 0.0f), g_modelMatrix);
#else
	// normal
	output.normal = mul(float4(input.normal, 0.0f), g_modelMatrix);
#endif
	
	return output;
}

// Pixel Shader
float4 PSMain(VSOutput input) : SV_Target
{
	//return float4(input.normal, 1.0f);

	const float3 kSpecColor = float3(2.0f, 2.0f, 2.0f);
	const float kSpecPower = 256.0f;

	float3 ambientColor = g_DirLight_ambientColor.xyz;
	float3 albedoColor = materialAlbedo(g_albedoTexture, g_albedoSampler, input.texcoord);

#ifdef UNLIT
	return float4(albedoColor, 1.0f);
#endif

	float3 N = input.normal;
	float3 L = -g_DirLight_direction.xyz;
	float3 V = normalize(g_viewPos.xyz - input.worldPos);
	float3 H = normalize(L + V);

	float NdotL = max(0.0f, dot(N, L));
	float NdotH = max(0.0f, dot(N, H));

	float3 diffuseColor = ambientColor + NdotL * g_DirLight_color.rgb;
	float3 specularColor  = pow(NdotH, kSpecPower) * kSpecColor;
	
	float3 ligthing = float3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < g_pointLightCount; i++)
	{
		// get local copy of point light from ubo
		PointLight pointLight = g_pointLights[i];
		
		float3 PL = normalize(pointLight.position.xyz - input.worldPos);
		float3 R = reflect(-PL, input.normal);
		
		// we store point light radius in r component
		float radius = pointLight.lightData.r;
		
		// light attenuation
		float lightDistance = length(pointLight.position.xyz - input.worldPos) / radius;
		float attenuation = 1.0 / (1.0 + lightDistance * lightDistance);
		
		ligthing += pointLightPhong(pointLight.color.rgb, pointLight.specular.rgb, 2.0, PL, input.normal, R) * attenuation;
	}
	
	//float3 finalColor = albedoColor * (diffuseColor + specularColor);
	float3 finalColor = albedoColor * (diffuseColor + specularColor + ligthing);
	//float3 finalColor = albedoColor * (ligthing);
	return float4(finalColor, 1.0f);
}
