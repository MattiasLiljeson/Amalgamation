cbuffer cbFixed
{
	float2 gQuadTexC[4] = 
	{
		float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f)
	};
};

cbuffer cbPerFrame : register(b0)
{
	float4 gEyePosW;
	matrix gView; 
    matrix gProj;
	float4 Color;
	float fadeIn;
	float fadeOut;
	float particleMaxAge;
	float maxOpacity;
	float4 CameraZ;
	float4 CameraY;
	int Alignment;
};

Texture2D Texture : register(t0);
SamplerState SampleType : register(s0);

struct Particle
{
	float3 Position        : POSITION;
	float3 Velocity        : VELOCITY;
	float2 Size            : SIZE;
	float Age			   : AGE;
	float AngularVelocity  : ANGULARVELOCITY;
	float Rotation		   : ROTATION;
};
struct GS_OUT
{
	float4 posH  : SV_POSITION;
	float2 texC  : TEXCOORD;
	float4 color : COLOR;
};


Particle VS(Particle vIn)
{
	return vIn;
}

[maxvertexcount(4)]
void GS(point Particle gIn[1], 
            inout TriangleStream<GS_OUT> triStream)
{		
	matrix W;
	if (Alignment == 0) //Observer
	{
		float3 look  = normalize(gEyePosW.xyz - gIn[0].Position);
		float3 right = normalize(cross(float3(0,1,0), look));
		float3 up    = cross(look, right);
		W[0] = float4(right,       0.0f);
		W[1] = float4(up,          0.0f);
		W[2] = float4(look,        0.0f);
		W[3] = float4(gIn[0].Position, 1.0f);
	}
	else if (Alignment == 1) //Screen
	{
		float3 look  = -CameraZ.xyz;
		float3 up    = CameraY.xyz;
		float3 right = normalize(cross(up, look));
		W[0] = float4(right,       0.0f);
		W[1] = float4(up,          0.0f);
		W[2] = float4(look,        0.0f);
		W[3] = float4(gIn[0].Position, 1.0f);
	}
	else if (Alignment == 2) //World Up
	{
		float3 up 	 = float3(0, 1, 0);
		float3 right = normalize(cross(up, gEyePosW.xyz - gIn[0].Position));
		float3 look  = cross(right, up);
		W[0] = float4(right,       0.0f);
		W[1] = float4(up,          0.0f);
		W[2] = float4(look,        0.0f);
		W[3] = float4(gIn[0].Position, 1.0f);
	}
	else //Velocity
	{
		float3 right = normalize(gIn[0].Velocity);
		float3 up 	 = normalize(cross(gEyePosW.xyz - gIn[0].Position, right));
		float3 look  = cross(right, up);
		W[0] = float4(right,       0.0f);
		W[1] = float4(up,          0.0f);
		W[2] = float4(look,        0.0f);
		W[3] = float4(gIn[0].Position, 1.0f);
	}
	
	
	matrix rot = matrix(cos(gIn[0].Rotation), -sin(gIn[0].Rotation), 0, 0,
						sin(gIn[0].Rotation), cos(gIn[0].Rotation), 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1);

	W = mul(rot, W);
	matrix vp = mul(gView, gProj);
	matrix WVP = mul(W, vp);
	
	float halfWidth  = 0.5f*gIn[0].Size.x;
	float halfHeight = 0.5f*gIn[0].Size.y;

	float4 v[4];
	v[0] = float4(-halfWidth, -halfHeight, 0.0f, 1.0f);
	v[1] = float4(+halfWidth, -halfHeight, 0.0f, 1.0f);
	v[2] = float4(-halfWidth, +halfHeight, 0.0f, 1.0f);
	v[3] = float4(+halfWidth, +halfHeight, 0.0f, 1.0f);
	float2 t[4];
	t[0] = float2(0.0f, 1.0f);
	t[1] = float2(1.0f, 1.0f);
	t[2] = float2(0.0f, 0.0f);
	t[3] = float2(1.0f, 0.0f);
	
	float opacity;
	if (gIn[0].Age < fadeIn) //Fade in
		opacity = smoothstep(0.0f, 1.0f, gIn[0].Age / fadeIn); 
	else if (gIn[0].Age > fadeOut)//Fade out
		opacity = (particleMaxAge - gIn[0].Age) / (particleMaxAge - fadeOut);
	else
		opacity = 1.0f;
	
	opacity *= maxOpacity;
	GS_OUT gOut;
	[unroll]
	for(int i = 0; i < 4; ++i)
	{
		gOut.posH  = mul(v[i], WVP);
		gOut.texC  = t[i];
		gOut.color = float4(Color.x, Color.y, Color.z, opacity);
		triStream.Append(gOut);
	}	
}

float4 PS(GS_OUT pIn) : SV_TARGET
{
	float4 color = Texture.Sample(SampleType, pIn.texC);
	return color;
	
	//color *= pIn.color;
	//return float4(1,0,0,1);
}