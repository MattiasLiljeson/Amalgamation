// temporary camera matrix helpers
#pragma once

#include <AglMatrix.h>
#include <AglVector3.h>
class CHANGETHIS
{
public:

static void SetLookAtMatrix(AglMatrix&  p_outMat, const AglVector3& p_eye, 
							   const AglVector3& p_at, const AglVector3& p_up)
{
	AglVector3 zaxis,xaxis,yaxis;
	// z-axis
	zaxis = p_at - p_eye;					  // subtract at vector with eye vector
	AglVector3::normalize(zaxis);

	// x-axis
	xaxis = AglVector3::crossProduct(p_up,zaxis);
	AglVector3::normalize(xaxis);
	// y-axis
	yaxis = AglVector3::crossProduct(zaxis,xaxis);	

	p_outMat = AglMatrix(
	xaxis.x,           yaxis.x,           zaxis.x,           0.0f,
	xaxis.y,           yaxis.y,           zaxis.y,           0.0f,
	xaxis.z,           yaxis.z,           zaxis.z,           0.0f,
	-AglVector3::dotProduct(xaxis, p_eye),  -AglVector3::dotProduct(yaxis, p_eye),  -AglVector3::dotProduct(zaxis, p_eye),  1.0f
	);

}



static void SetProjMatrix(AglMatrix&  p_outMat, float p_yFOV, float p_aspectRatio,
									float p_nearClip, float p_farClip)
{
	float yScale = 1.0f/tan(p_yFOV/2.0f);
	float xScale = yScale / p_aspectRatio;

	p_outMat = AglMatrix(
	xScale,     0.0f,          0.0f,									       0.0f,
	0.0f,        yScale,       0.0f,										   0.0f,
	0.0f,          0.0f,       p_farClip/(p_farClip-p_nearClip),			   1.0f,
	0.0f,          0.0f,       -p_nearClip*p_farClip/(p_farClip-p_nearClip),   0.0f
	);	
}

};