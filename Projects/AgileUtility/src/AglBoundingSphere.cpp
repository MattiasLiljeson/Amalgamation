#include "AglBoundingSphere.h"

AglBoundingSphere::AglBoundingSphere()
{
	position = AglVector3(0, 0, 0);
	radius = 0;
}
AglBoundingSphere::AglBoundingSphere(AglVector3 p_position, float p_radius)
{
	position = p_position;
	radius = p_radius;
}
AglBoundingSphere::AglBoundingSphere(AglVector3 p_p1, AglVector3 p_p2)
{
	position = (p_p1 + p_p2) * 0.5f;
	radius = AglVector3::length(p_p2-p_p1) * 0.5f;
}
AglBoundingSphere::AglBoundingSphere(AglVector3 p_p1, AglVector3 p_p2, AglVector3 p_p3)
{
	AglVector3 oldP2 = p_p2;
	AglVector3 oldP3 = p_p3;

	//Move p_p1 to origo
	p_p2 -= p_p1;
	p_p3 -= p_p1;

	//Transform to 2d
	AglVector3 n = AglVector3::crossProduct(p_p2, p_p3);
	AglQuaternion q = AglQuaternion::rotateToFrom(n, AglVector3(0, 0, 1));
	q.transformVector(p_p2);
	q.transformVector(p_p3);

	//Find 2 lines colliding at the circumcircle center
	AglVector2 a = AglVector2(p_p2[0], p_p2[1]) * 0.5f;
	AglVector2 c = AglVector2(p_p3[0], p_p3[1]) * 0.5f;
	AglVector2 b = a + AglVector2(-p_p2[1], p_p2[0]) * 10;
	AglVector2 d = c + AglVector2(-p_p3[1], p_p3[0]) * 10;

	//Compute the lines collision
	AglVector2 c2d;
	Agl2DLine::LineLineCollisionPoint(Agl2DLine(a, b), Agl2DLine(c, d), c2d);
	AglVector3 center(c2d[0], c2d[1], 0);

	//Transform back to 3d
	AglQuaternion q2 = AglQuaternion::rotateToFrom(AglVector3(0, 0, 1), n);
	q2.transformVector(center);
	center += p_p1;
	float r = AglVector3::length(p_p1 - center);

	float r2 = AglVector3::length(oldP2 - center);
	float r3 = AglVector3::length(oldP3 - center);

	if (abs(r2 -r) > 0.0001f)
		radius = 0;
	if (abs(r3 -r) > 0.0001f)
		radius = 0;

	//Construct sphere
	position = center;
	radius = r;
}
AglBoundingSphere::AglBoundingSphere(AglVector3 p_p1, AglVector3 p_p2, AglVector3 p_p3, AglVector3 p_p4)
{
	float a1 = p_p1.x;
	float a2 = p_p1.y;
	float a3 = p_p1.z;
	float b1 = p_p2.x;
	float b2 = p_p2.y;
	float b3 = p_p2.z;
	float c1 = p_p3.x;
	float c2 = p_p3.y;
	float c3 = p_p3.z;
	float d1 = p_p4.x;
	float d2 = p_p4.y;
	float d3 = p_p4.z;

	float x =((a1*a1+a2*a2+a3*a3)*(c2*d3 + d2*b3 + b2*c3 - d2*c3 - b2*d3 - c2*b3) +
		(b1*b1+b2*b2+b3*b3)*(d2*c3 + c2*a3 + a2*d3 - c2*d3 - a2*c3 - d2*a3) +
		(c1*c1+c2*c2+c3*c3)*(a2*b3 + b2*d3 + d2*a3 - b2*a3 - d2*b3 - a2*d3) +
		(d1*d1+d2*d2+d3*d3)*(b2*a3 + a2*c3 + c2*b3 - a2*b3 - c2*a3 - b2*c3)) /
		(2*((a1)*(c2*d3 + d2*b3 + b2*c3 - d2*c3 - b2*d3 - c2*b3) +
		(b1)*(d2*c3 + c2*a3 + a2*d3 - c2*d3 - a2*c3 - d2*a3) +
		(c1)*(a2*b3 + b2*d3 + d2*a3 - b2*a3 - d2*b3 - a2*d3) +
		(d1)*(b2*a3 + a2*c3 + c2*b3 - a2*b3 - c2*a3 - b2*c3)));

	float y =((a2*a2+a3*a3+a1*a1)*(c3*d1 + d3*b1 + b3*c1 - d3*c1 - b3*d1 - c3*b1) +
		(b2*b2+b3*b3+b1*b1)*(d3*c1 + c3*a1 + a3*d1 - c3*d1 - a3*c1 - d3*a1) +
		(c2*c2+c3*c3+c1*c1)*(a3*b1 + b3*d1 + d3*a1 - b3*a1 - d3*b1 - a3*d1) +
		(d2*d2+d3*d3+d1*d1)*(b3*a1 + a3*c1 + c3*b1 - a3*b1 - c3*a1 - b3*c1)) /
		(2*((a2)*(c3*d1 + d3*b1 + b3*c1 - d3*c1 - b3*d1 - c3*b1) +
		(b2)*(d3*c1 + c3*a1 + a3*d1 - c3*d1 - a3*c1 - d3*a1) +
		(c2)*(a3*b1 + b3*d1 + d3*a1 - b3*a1 - d3*b1 - a3*d1) +
		(d2)*(b3*a1 + a3*c1 + c3*b1 - a3*b1 - c3*a1 - b3*c1)));

	float z =  ((a3*a3+a1*a1+a2*a2)*(c1*d2 + d1*b2 + b1*c2 - d1*c2 - b1*d2 - c1*b2) +
				(b3*b3+b1*b1+b2*b2)*(d1*c2 + c1*a2 + a1*d2 - c1*d2 - a1*c2 - d1*a2) +
				(c3*c3+c1*c1+c2*c2)*(a1*b2 + b1*d2 + d1*a2 - b1*a2 - d1*b2 - a1*d2) +
				(d3*d3+d1*d1+d2*d2)*(b1*a2 + a1*c2 + c1*b2 - a1*b2 - c1*a2 - b1*c2)) /
				(2*((a3)*(c1*d2 + d1*b2 + b1*c2 - d1*c2 - b1*d2 - c1*b2) +
				(b3)*(d1*c2 + c1*a2 + a1*d2 - c1*d2 - a1*c2 - d1*a2) +
				(c3)*(a1*b2 + b1*d2 + d1*a2 - b1*a2 - d1*b2 - a1*d2) +
				(d3)*(b1*a2 + a1*c2 + c1*b2 - a1*b2 - c1*a2 - b1*c2)));
	position = AglVector3(x, y, z);
	radius = AglVector3::length(position - p_p1);
	float radius2 = AglVector3::length(position - p_p2);
	float radius3 = AglVector3::length(position - p_p3);
	float radius4 = AglVector3::length(position - p_p4);
	if (abs(radius2 - radius) > 0.001f)
		radius = 0;
	if (abs(radius3 - radius) > 0.001f)
		radius = 0;
	if (abs(radius4 - radius) > 0.001f)
		radius = 0;
}												



bool AglBoundingSphere::pointInSphere(AglVector3 p_point)
{
	AglVector3 dir = p_point - position;
	if (AglVector3::lengthSquared(dir) < radius * radius)
		return true;
	return false;
}
bool AglBoundingSphere::pointInSphereMargin(AglVector3 p_point, float p_margin)
{
	AglVector3 dir = p_point - position;
	if (AglVector3::lengthSquared(dir) < (radius + p_margin) * (radius + p_margin))
		return true;
	return false;
}

AglBoundingSphere AglBoundingSphere::minimumBoundingSphere(const vector<AglVector3>& p_points)
{
	vector<AglVector3> span(4);
	return welzlComputation(p_points, p_points.size(), span, 0);
}
AglBoundingSphere AglBoundingSphere::welzlComputation(const vector<AglVector3>& p_points, unsigned int p_size, 
		vector<AglVector3>& p_span, unsigned int p_spanSize)
{
	if (p_size == 0 || p_spanSize == 4)
	{
		switch (p_spanSize)
		{
		case 0:
			return AglBoundingSphere();
		case 1:
			return AglBoundingSphere(p_span[0], 0);
		case 2:
			return AglBoundingSphere(p_span[0], p_span[1]);
		case 3:
			return AglBoundingSphere(p_span[0], p_span[1], p_span[2]);
		case 4:
			return AglBoundingSphere(p_span[0], p_span[1], p_span[2], p_span[3]);
		}
	}

	unsigned int index = p_size - 1;

	AglBoundingSphere s = welzlComputation(p_points, p_size - 1, p_span, p_spanSize);

	if (s.pointInSphereMargin(p_points[index], 0.0001f))
		return s;

	p_span[p_spanSize] = p_points[index];

	return welzlComputation(p_points, p_size - 1, p_span, p_spanSize + 1);
}