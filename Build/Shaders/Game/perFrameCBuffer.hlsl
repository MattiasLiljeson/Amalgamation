// Total of 192 bytes
cbuffer perFrame: register(b0)
{
	float4x4 	gViewProj;			//64 bytes 	c0,c1,c2,c3
	float4x4 	gViewProjInverse;	//64 bytes	c4,c5,c6,c7
	float4		gCameraPos;			//16 bytes	c8
	float4		gCameraForward;		//16 bytes	c9
	float4		gCameraUp;			//16 bytes	c10
	float2		gRenderTargetSize;	//8 bytes	c11.xy
	float		gFarPlane;			//4 bytes	c11.z
	float		gNearPlane;			//4 bytes	c11.w
};