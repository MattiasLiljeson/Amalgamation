// =======================================================================================
//                                      AxisPruner
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # AxisPruner
/// Detailed description.....
/// Created on: 6-2-2013 
///---------------------------------------------------------------------------------------
#ifndef AXISPRUNER_H
#define AXISPRUNER_H

#include "RigidBody.h"
#include <vector>

using namespace std;

struct Interval
{
	RigidBody* body;
	float minPx;
	float maxPx;
	float minPy;
	float maxPy;
	float minPz;
	float maxPz;

	Interval(RigidBody* pBody, float pMinPx, float pMaxPx, float pMinPy, float pMaxPy, float pMinPz, float pMaxPz)
	{
		body = pBody;
		minPx = pMinPx;
		maxPx = pMaxPx;
		minPy = pMinPy;
		maxPy = pMaxPy;
		minPz = pMinPz;
		maxPz = pMaxPz;
	}
};

struct IntervalAxis
{
	AglVector3 axis;
	vector<Interval*> sortedList;

	IntervalAxis(AglVector3 pAxis)
	{
		axis = pAxis;
	}
};

struct IntProjection
{
	float xMin;
	float yMin;
	float zMin;
	float xMax;
	float yMax;
	float zMax;
};

class AxisPruner
{
private:
	IntervalAxis*  mAxis;

private:
	Interval* Project(RigidBody* pBody);
	void Project(RigidBody* pBody, IntProjection* pProjection);
	void Insert(Interval* pInterval);

public:
	AxisPruner();
	virtual ~AxisPruner();

	void Insert(RigidBody* pBody);
	vector<PhyCollisionData> Query(RigidBody* pBody);
};

#endif // AXISPRUNER_H