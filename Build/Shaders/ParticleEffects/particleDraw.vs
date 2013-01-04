struct Particle
{
	float3 Position        : POSITION;
	float3 Velocity        : VELOCITY;
	float Size             : SIZE;
	float Age			   : AGE;
};
  
struct VS_OUT
{
	float3 Position  : POSITION;
	float  Size : SIZE;
	float  Age	: AGE;
};

VS_OUT VShader(Particle vIn)
{
	VS_OUT vOut;
	vOut.Position = vIn.Position;
	vOut.Size = vIn.Size;
	vOut.Age = vIn.Age;
	return vOut;
}