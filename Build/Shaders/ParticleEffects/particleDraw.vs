struct Particle
{
	float3 Position			: POSITION;
	float Age				: AGE;
	float3 Velocity			: VELOCITY;
	float AngularVelocity	: ANGULARVELOCITY;
	float2 Size				: SIZE;
	float Rotation			: ROTATION;
	float pad				: PAD;
};
  
struct VS_OUT
{
	float3 Position  : POSITION;
	float2  Size : SIZE;
	float  Age	: AGE;
	float3 Velocity : VELOCITY;
	float AngularVelocity  : ANGULARVELOCITY;
	float Rotation		   : ROTATION;
};

VS_OUT VShader(Particle vIn)
{
	VS_OUT vOut;
	vOut.Position = vIn.Position;
	vOut.Size = vIn.Size;
	vOut.Age = vIn.Age;
	vOut.Velocity = vIn.Velocity;
	vOut.AngularVelocity = vIn.AngularVelocity;
	vOut.Rotation = vIn.Rotation;
	return vOut;
}