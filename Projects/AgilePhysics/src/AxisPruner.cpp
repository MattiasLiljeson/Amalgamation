#include "AxisPruner.h"
#include "RigidBodyMesh.h"
#include "RigidBodyBox.h"
#include "CollisionManager.h"

Interval* AxisPruner::Project(RigidBody* pBody)
{
	if (pBody->GetType() == MESH)
	{
		AglOBB obb = ((RigidBodyMesh*)pBody)->GetOBB();
		vector<AglVector3> corn = obb.getCorners();

		AglVector3 axes[] = { AglVector3(1, 0, 0), AglVector3(0, 1, 0), AglVector3(0, 0, 1) };
		float minP[3], maxP[3];
		for (unsigned int axis = 0; axis < 3; axis++)
		{
			minP[axis] = maxP[axis] = AglVector3::dotProduct(corn[0], axes[axis]); 
			for (unsigned int i = 1; i < 8; i++)
			{
				float val = AglVector3::dotProduct(corn[i], axes[axis]);
				minP[axis] = min(val, minP[axis]);
				maxP[axis] = max(val, maxP[axis]);
			}
		}
		return new Interval(pBody, minP[0], maxP[0], minP[1], maxP[1], minP[2], maxP[2]);
	}
	int k = 0;
	k = 1 / k;
	return NULL;
}
void AxisPruner::Project(RigidBody* pBody, IntProjection* pProjection)
{
	if (pBody->GetType() == BOX)
	{
		vector<AglVector3> corn = ((RigidBodyBox*)pBody)->GetCorners();

		AglVector3 axes[] = { AglVector3(1, 0, 0), AglVector3(0, 1, 0), AglVector3(0, 0, 1) };
		float minP[3], maxP[3];
		for (unsigned int axis = 0; axis < 3; axis++)
		{
			minP[axis] = maxP[axis] = AglVector3::dotProduct(corn[0], axes[axis]); 
			for (unsigned int i = 1; i < 8; i++)
			{
				float val = AglVector3::dotProduct(corn[i], axes[axis]);
				minP[axis] = min(val, minP[axis]);
				maxP[axis] = max(val, maxP[axis]);
			}
		}
		pProjection->xMin = minP[0];
		pProjection->yMin = minP[1];
		pProjection->zMin = minP[2];
		pProjection->xMax = maxP[0];
		pProjection->yMax = maxP[1];
		pProjection->zMax = maxP[2];
		return;
	}
	int k = 0;
	k = 1 / k;
}
void AxisPruner::Insert(Interval* pInterval)
{
	//Mains the inteval sorted from smallest to largest max value
	mAxis->sortedList.push_back(pInterval);

	for (unsigned int i = mAxis->sortedList.size()-1; i > 0; i--)
	{
		if (mAxis->sortedList[i]->minPx < mAxis->sortedList[i-1]->minPx)
		{
			Interval* temp = mAxis->sortedList[i];
			mAxis->sortedList[i] = mAxis->sortedList[i-1];
			mAxis->sortedList[i-1] = temp;
		}
		else
		{
			break;
		}
	}
}

AxisPruner::AxisPruner()
{
	mAxis = new IntervalAxis(AglVector3(1, 0, 0));
}

AxisPruner::~AxisPruner()
{
	delete mAxis;
}

void AxisPruner::Insert( RigidBody* pBody )
{
	Interval* Int1 = Project(pBody);
	Insert(Int1);
}

vector<PhyCollisionData> AxisPruner::Query( RigidBody* pBody )
{
	vector<PhyCollisionData> collisions;

	if (mAxis->sortedList.size() == 0)
		return collisions;

	IntProjection proj;
	Project(pBody, &proj);

	//LOCATE START
	//Should switch to binary search later

	int limit = mAxis->sortedList.size()-1;

	int loc = 0;
	while (proj.xMax < mAxis->sortedList[loc]->minPx && loc < limit)
		loc++;

	int counter = 0;
	//FIND INTERVAL
	while (loc <= limit && proj.xMax > mAxis->sortedList[loc]->minPx)
	{
		if (proj.xMin < mAxis->sortedList[loc]->maxPx)
		{
			//Collides along x

			if (proj.yMax > mAxis->sortedList[loc]->minPy)
			{
				if (proj.yMin < mAxis->sortedList[loc]->maxPy)
				{
					//Collides along y
					if (proj.zMax > mAxis->sortedList[loc]->minPz)
					{
						if (proj.zMin < mAxis->sortedList[loc]->maxPz)
						{
							counter++;
							//Collides along z
							PhyCollisionData colData;
							if (CheckCollision(pBody, mAxis->sortedList[loc]->body, &colData))
							{
								collisions.push_back(colData);
							}
						}
					}
				}
			}
		}
		loc++;
	}
	return collisions;
}
