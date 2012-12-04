#ifndef AGL2DLINE_H
#define AGL2DLINE_H

#include "AglVector2.h"

struct Agl2DLine
{
	AglVector2 p1;
	AglVector2 p2;

	Agl2DLine(AglVector2 p_p1, AglVector2 p_p2)
	{
		p1 = p_p1;
		p2 = p_p2;
	}

	static bool LineLineCollisionPoint(Agl2DLine p_l1, Agl2DLine p_l2, AglVector2& p_point)
	{
		float denom = (p_l1.p1[0] - p_l1.p2[0]) * (p_l2.p1[1] - p_l2.p2[1]) -
						(p_l1.p1[1] - p_l1.p2[1]) * (p_l2.p1[0] - p_l2.p2[0]);
		if (denom == 0)
			return false;

		float nomX = (p_l1.p1[0]*p_l1.p2[1] - p_l1.p1[1]*p_l1.p2[0])*(p_l2.p1[0] - p_l2.p2[0]) -
					 (p_l2.p1[0]*p_l2.p2[1] - p_l2.p1[1]*p_l2.p2[0])*(p_l1.p1[0] - p_l1.p2[0]);
							 			
		float nomY = (p_l1.p1[0]*p_l1.p2[1] - p_l1.p1[1]*p_l1.p2[0])*(p_l2.p1[1] - p_l2.p2[1]) -
					 (p_l2.p1[0]*p_l2.p2[1] - p_l2.p1[1]*p_l2.p2[0])*(p_l1.p1[1] - p_l1.p2[1]);
		
		float frac = 1.0f / denom;
		p_point = AglVector2(nomX*frac, nomY*frac);
		return true;
	}
};

#endif