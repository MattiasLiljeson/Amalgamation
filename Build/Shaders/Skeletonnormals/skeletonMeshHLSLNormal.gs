cbuffer ConstBuffer
{
    matrix View;
    matrix Proj;
	float4 Length;
};

struct GEO_IN
{
    float3 wpos         : POSITION;
	float3 normal	    : NORMAL;
};

struct GEO_OUT
{
    float4 Position : SV_POSITION;
};

struct PS_OUTPUT 
{
    float4 Color : SV_Target0;
};

[maxvertexcount(2)]
void GS(point GEO_IN data[1], inout LineStream<GEO_OUT> output)
{
    float3 p0 = data[0].wpos;
    float3 p1 = data[0].wpos + data[0].normal*Length[0];

	GEO_OUT v[2];
	
	v[0].Position = mul(float4(p0, 1), View);
    v[0].Position = mul(v[0].Position, Proj);
	
	v[1].Position = mul(float4(p1, 1), View);
    v[1].Position = mul(v[1].Position, Proj);
	
    output.Append(v[0]);
    output.Append(v[1]);
}