#include "SpawnPointSet.h"

SpawnPointSet::SpawnPointSet() : Component(ComponentType::SpawnPointSet)
{

}

SpawnPointSet::SpawnPointSet( const vector<SpawnPoint>& p_spawnPoints ) : Component(ComponentType::SpawnPointSet)
{
	m_spawnPoints = p_spawnPoints;
}
SpawnPointSet::SpawnPointSet(const vector<pair<string, AglMatrix>>& p_spawnPoints) : Component(ComponentType::SpawnPointSet)
{
	for (unsigned int i = 0; i < p_spawnPoints.size(); i++)
	{
		m_spawnPoints.push_back(SpawnPoint(p_spawnPoints[i].second, p_spawnPoints[i].first));
	}
}

SpawnPointSet::~SpawnPointSet()
{

}
