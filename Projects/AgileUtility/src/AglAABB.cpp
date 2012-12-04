#include "AglAABB.h"

AglAABB::AglAABB(AglVector3 p_p1, AglVector3 p_p2)
{
	minimum = AglVector3(min(p_p1.x, p_p2.x), min(p_p1.y, p_p2.y), min(p_p1.z, p_p2.z));
	maximum = AglVector3(max(p_p1.x, p_p2.x), max(p_p1.y, p_p2.y), max(p_p1.z, p_p2.z));
}

AglAABB AglAABB::constructMinimum(vector<AglVector3> p_points)
{
	AglVector3 minP, maxP;
	minP = maxP = p_points[0];
	for (unsigned int i = 1; i < p_points.size(); i++)
	{
		minP = AglVector3(min(minP.x, p_points[i].x), min(minP.y, p_points[i].y), min(minP.z, p_points[i].z));
		maxP = AglVector3(max(maxP.x, p_points[i].x), max(maxP.y, p_points[i].y), max(maxP.z, p_points[i].z));
	}
	return AglAABB(minP, maxP);
}