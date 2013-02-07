#include "ConnectionPointSet.h"

ConnectionPointSet::ConnectionPointSet()
	: Component( ComponentType::ConnectionPointSet )
{

}

ConnectionPointSet::ConnectionPointSet(const vector<ConnectionPoint>& p_connectionPoints)
	: Component( ComponentType::ConnectionPointSet )
{
	m_connectionPoints=p_connectionPoints;
}

ConnectionPointSet::ConnectionPointSet(const vector<AglMatrix>& p_connectionPoints)
	: Component( ComponentType::ConnectionPointSet )
{
	for (int i=0;i<p_connectionPoints.size();i++)
	{
		m_connectionPoints.push_back(p_connectionPoints[i]);
	}

}

ConnectionPointSet::~ConnectionPointSet()
{

}