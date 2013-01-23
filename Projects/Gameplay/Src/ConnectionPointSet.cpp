#include "ConnectionPointSet.h"

ConnectionPointSet::ConnectionPointSet()
{
	m_highlighted = 0;
}

ConnectionPointSet::ConnectionPointSet(const vector<ConnectionPoint>& p_connectionPoints)
{
	m_connectionPoints=p_connectionPoints;
	m_highlighted = 0;
}

ConnectionPointSet::ConnectionPointSet(const vector<AglMatrix>& p_connectionPoints)
{
	for (int i=0;i<p_connectionPoints.size();i++)
	{
		m_connectionPoints.push_back(p_connectionPoints[i]);
	}
	
	m_highlighted = 0;
}

ConnectionPointSet::~ConnectionPointSet()
{

}