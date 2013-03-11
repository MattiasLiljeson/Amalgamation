#include "perFrameCBuffer.hlsl"
#include "utility.hlsl"

static const float g_LIGHT_MULT = 10;

Texture2D g_diffuse				: register(t0);
Texture2D g_normal				: register(t1);
Texture2D g_specular			: register(t2);
Texture2D g_diffLight 			: register(t3);
Texture2D g_specLight			: register(t4);

Texture2D g_depth				: register(t10);

SamplerState g_samplerPointWrap : register(s0);

struct VertexIn
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD; 
};
struct VertexOut
{
    float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
};

float3 getPosition(float2 p_uv,float p_depth) 
{
	return getWorldPosFromTexCoord( p_uv, p_depth, gViewProjInverse);
}

VertexOut VS(VertexIn p_input)
{
	VertexOut vout;
	vout.position = float4(p_input.position,1.0f);
	vout.texCoord = p_input.texCoord;
    
	return vout;
}

float4 PS(VertexOut input) : SV_TARGET
{
	uint3 index;
	index.xy = input.position.xy;
	index.z = 0;

	float depth = g_depth.Load( index ).r;

	float3 fog = gFogColorAndFogFar.rgb;
	float3 ambient = gAmbientColorAndFogNear.rgb;
	float2 fogNearFarPercentage = float2(gAmbientColorAndFogNear.a,gFogColorAndFogFar.a);
	float3 position = getPosition(input.texCoord,depth);

	// calc linear depths
	float pixelDepthW = length(position-gCameraPos.xyz);
	//float linDepth = pixelDepthW / (gFarPlane-gNearPlane);
	fogNearFarPercentage.y=0.4f;
	float fogDepth = saturate(pixelDepthW / (gFarPlane*fogNearFarPercentage.y-gNearPlane));
	// saturate(pixelDepthW / (gFarPlane*fogNearFarPercentage.x-gNearPlane*(2.0f-fogNearFarPercentage.y)));
	
	float4 sampledGlow = g_diffuse.Load( index ).rgba;
	float3 finalEmissiveValue = (sampledGlow.rgb * sampledGlow.a).rgb;
	float4 diffBuff = g_diffuse.Load( index );
	float4 diffLight = g_diffLight.Load( index ) * g_LIGHT_MULT;
	float4 specBuff = g_specular.Load( index );
	float4 specLight = g_specLight.Load( index ) * g_LIGHT_MULT;
	float4 finalCol = diffBuff*diffLight + specBuff*specLight;

	finalCol += float4 (ambient,0.0f );	
	// apply fog
	finalCol = float4( lerp( finalCol.rgb, fog, fogDepth), finalCol.a ); 
	// apply glow
	finalCol += float4( finalEmissiveValue, 0.0f );
	
	return float4( finalCol.rgb, 1.0f );
}