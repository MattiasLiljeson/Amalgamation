#include "AglCollision.h"

bool AglCollision::isColliding(AglBoundingSphere p_sphere1, AglBoundingSphere p_sphere2)
{
	AglVector3 StoS = p_sphere2.position-p_sphere1.position;
	float twoRadius = p_sphere1.radius + p_sphere2.radius;
	if (AglVector3::lengthSquared(StoS) < twoRadius*twoRadius)
		return true;
	return false;
}
bool AglCollision::isColliding(AglBoundingSphere p_sphere, AglOBB p_obb)
{
	//Find the closest point on the box to the sphere
	AglVector3 point = p_obb.world.GetTranslation();
	AglVector3 dir = p_sphere.position - point;

	vector<AglVector3> axes = p_obb.getCorners();
	AglVector3 size = p_obb.size;

	//Axis 1
	float proj = AglVector3::dotProduct(dir, axes[0]);
	proj = max(min(proj, size.x), -size.x);
	point += axes[0] * proj;

	//Axis 2
	proj = AglVector3::dotProduct(dir, axes[1]);
	proj = max(min(proj, size.y), -size.y);
	point += axes[1] * proj;

	//Axis 3
	proj = AglVector3::dotProduct(dir, axes[2]);
	proj = max(min(proj, size.z), -size.z);
	point += axes[2] * proj;

	//Check the distance to this closest point
	if (AglVector3::lengthSquared(p_sphere.position - point) < p_sphere.radius * p_sphere.radius)
	{
		return true;
	}
	return false;
}
bool AglCollision::isColliding(AglOBB p_obb1, AglOBB p_obb2)
{
	AglVector3 aaxes[3];
	aaxes[0] = p_obb1.world.GetRight();
	aaxes[1] = p_obb1.world.GetForward();
	aaxes[2] = p_obb1.world.GetUp();

	AglVector3 baxes[3];
	baxes[0] = p_obb2.world.GetRight();
	baxes[1] = p_obb2.world.GetForward();
	baxes[2] = p_obb2.world.GetUp();

	AglVector3 axes[15];
	axes[0] = aaxes[0];
	axes[1] = aaxes[1];
	axes[2] = aaxes[2];
	axes[3] = baxes[0];
	axes[4] = baxes[1];
	axes[5] = baxes[2];
	axes[6] = AglVector3::crossProduct(aaxes[0], baxes[0]);
	axes[7] = AglVector3::crossProduct(aaxes[0], baxes[1]);
	axes[8] = AglVector3::crossProduct(aaxes[0], baxes[2]);
	axes[9] = AglVector3::crossProduct(aaxes[1], baxes[0]);
	axes[10] = AglVector3::crossProduct(aaxes[1], baxes[1]);
	axes[11] = AglVector3::crossProduct(aaxes[1], baxes[2]);
	axes[12] = AglVector3::crossProduct(aaxes[2], baxes[0]);
	axes[13] = AglVector3::crossProduct(aaxes[2], baxes[1]);
	axes[14] = AglVector3::crossProduct(aaxes[2], baxes[2]);

	float Penetration = FLT_MAX;
	AglVector3 Normal(0,0,0);

	vector<AglVector3> pointsA = p_obb1.getCorners();
	vector<AglVector3> pointsB = p_obb2.getCorners();
	for (int i = 0; i < 15; i++)
	{
		float overlap = overlapAmount(pointsA, pointsB, axes[i]);
		if (overlap <= 0)
			return false;
		else if (overlap < Penetration)
		{
			Penetration = overlap;
			Normal = axes[i];
		}
	}
	return true;
}

float AglCollision::overlapAmount(const vector<AglVector3>& p_points1, const vector<AglVector3>& p_points2, const AglVector3& p_axis)
{
	//Assumed to be normalized
	//AglVector3::normalize(pAxis);

	float minA = 0, maxA = 0, minB = 0, maxB = 0;
	calculateProjectionInterval(p_points1, p_axis, minA, maxA);
	calculateProjectionInterval(p_points2, p_axis, minB, maxB);

	float lengthA = maxA - minA;
	float lengthB = maxB - minB;

	float minTotal = min(minA, minB);
	float maxTotal = max(maxA, maxB);

	if (maxTotal - minTotal >= lengthA + lengthB)
	{
		return 0;
	}
	return lengthA + lengthB - (maxTotal - minTotal);
}

void AglCollision::calculateProjectionInterval(const vector<AglVector3>& p_points, const AglVector3& p_axis, 
								  float& p_min, float& p_max)
{
	p_min = p_max = AglVector3::dotProduct(p_axis, p_points[0]);
	for (int i = 1; i < 8; i++)
	{
		float curr = AglVector3::dotProduct(p_axis, p_points[i]);
		if (curr < p_min)
			p_min = curr;
		else if (curr > p_max)
			p_max = curr;
	}
}