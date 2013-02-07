#include "ConnectionPointSet.h"

ConnectionPointSet::ConnectionPointSet()
	: Component( ComponentType::ConnectionPointSet )
{
	m_highlighted = 0;
}

ConnectionPointSet::ConnectionPointSet(const vector<ConnectionPoint>& p_connectionPoints)
	: Component( ComponentType::ConnectionPointSet )
{
	m_connectionPoints=p_connectionPoints;
	m_highlighted = 0;
}

ConnectionPointSet::ConnectionPointSet(const vector<AglMatrix>& p_connectionPoints)
	: Component( ComponentType::ConnectionPointSet )
{
	for (int i=0;i<p_connectionPoints.size();i++)
	{
		ConnectionPoint cp(p_connectionPoints[i]);
		m_connectionPoints.push_back(cp);
	}
	
	m_highlighted = 0;
}

ConnectionPointSet::~ConnectionPointSet()
{

}