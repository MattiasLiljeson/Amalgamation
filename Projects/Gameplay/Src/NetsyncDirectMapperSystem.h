#pragma once

#include <EntitySystem.h>
#include <vector>
using namespace std;

// =======================================================================================
//	NetsyncDirectMapperSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Maps the networkIdentity of NetworkSynced entities with their entity id.
///        
/// # NetsyncDirectMapperSystem
/// Access time complexity is O(1) instead of O(n), but with the price of a small memory
/// overhead.
/// Created on: 24-1-2013 
///---------------------------------------------------------------------------------------
class NetsyncDirectMapperSystem: public EntitySystem
{
public:
	NetsyncDirectMapperSystem();
	~NetsyncDirectMapperSystem();
	void inserted( Entity* p_entity );
	void removed( Entity* p_entity );
	bool checkProcessing();
	void processEntities( const vector<Entity*>& p_entities );

	///-----------------------------------------------------------------------------------
	/// Returns the local entity of a given networkIdentity with O(1) complexity.
	/// \param p_networkIdentity
	/// \return Entity*
	///-----------------------------------------------------------------------------------
	Entity* getEntity( int p_networkIdentity );

private:
	vector<int> m_networkEntityIndices;

};