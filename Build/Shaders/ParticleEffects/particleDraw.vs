struct Particle
{
	float3 Position        : POSITION;
	float3 Velocity        : VELOCITY;
	float2 Size            : SIZE;
	float Age			   : AGE;
};
  
struct VS_OUT
{
	float3 Position  : POSITION;
	float2  Size : SIZE;
	float  Age	: AGE;
	float3 Velocity : VELOCITY;
};

VS_OUT VShader(Particle vIn)
{
	VS_OUT vOut;
	vOut.Position = vIn.Position;
	vOut.Size = vIn.Size;
	vOut.Age = vIn.Age;
	vOut.Velocity = vIn.Velocity;
	return vOut;
}