#ifndef COMMON_HLSL
#define COMMON_HLSL
static const float g_LIGHT_MULT = 10;

static const float blurFilter3[3][3] = {
	{0.01f,0.08f,0.01f},
	{0.08f,0.64f,0.01f},
	{0.01f,0.08f,0.01f}
};

static const float blurFilter5[5][5] = {
	{0.01f,0.02f,0.04f,0.02f,0.01f},
	{0.02f,0.04f,0.08f,0.04f,0.02f},
	{0.04f,0.08f,0.16f,0.08f,0.04f},
	{0.02f,0.04f,0.08f,0.04f,0.02f},
	{0.01f,0.02f,0.04f,0.02f,0.01f}
};

#define NUM_TAPS 12
static const float2 poisson[NUM_TAPS] = {
	{-.326,-.406},
	{-.840,-.074},
	{-.696, .457},
	{-.203, .621},
	{ .962,-.195},
	{ .473,-.480},
	{ .519, .767},
	{ .185,-.893},
	{ .507, .064},
	{ .896, .412},
	{-.322,-.933},
	{-.792,-.598}
};

Texture2D g_diffuse						: register(t0);
Texture2D g_normal						: register(t1);
Texture2D g_specular					: register(t2);
Texture2D g_diffLight 					: register(t3);
Texture2D g_specLight					: register(t4);

Texture2D g_diffuseLowRes				: register(t5);
Texture2D g_normalLowRes				: register(t6);
Texture2D g_specularLowRes				: register(t7);
Texture2D g_specLightLowRes 			: register(t8);
Texture2D g_diffLightLowRes				: register(t9);

Texture2D g_depth						: register(t10);

SamplerState g_samplerPointWrap 		: register(s0);
SamplerState g_samplerLinearWrap 		: register(s1);
SamplerState g_samplerAnisotropicWrap 	: register(s2);
SamplerState g_samplerPointClamp 		: register(s3);
SamplerState g_samplerLinearClamp 		: register(s4);
SamplerState g_samplerAnisotropicClamp 	: register(s5);

#endif