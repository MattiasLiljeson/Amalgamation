// Total of 288 bytes
cbuffer perFrame: register(b0)
{
	float4x4	gView;				//64 bytes
	float4x4 	gViewProj;			//64 bytes 	
	float4x4 	gViewProjInverse;	//64 bytes	
	float4		gCameraPos;			//16 bytes	
	float4		gCameraForward;		//16 bytes	
	float4		gCameraUp;			//16 bytes	
	float4		gAmbientColor;		//16 bytes	
	float4		gFogColor;			//16 bytes
	float2		gRenderTargetSize;	//8 bytes	
	float		gFarPlane;			//4 bytes	
	float		gNearPlane;			//4 bytes	
};

cbuffer shadowBuffer: register(b2)
{
	float4x4 shadowViewProj[2];		//128 bytes
}