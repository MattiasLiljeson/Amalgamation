#pragma once

#include <Entity.h>
#include <EntitySystem.h>
#include <AglMatrix.h>

using namespace std;
// =======================================================================================
//                              ConnectionVisualizerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ConnectionVisualizerSystem
/// Detailed description.....
/// Created on: 13-2-2013 
///---------------------------------------------------------------------------------------

class ConnectionVisualizerSystem : public EntitySystem
{
public:
	struct ConnectionEffectData
	{
		Entity*		parent;
		int			slot;
		AglVector3	offset;
		Entity*		data;
		bool		disabled;

		ConnectionEffectData(Entity* p_parent, int p_slot, AglVector3 p_offset)
		{
			parent = p_parent;
			slot = p_slot;
			offset = p_offset;
			disabled = false;
		}
	};


	ConnectionVisualizerSystem();
	virtual ~ConnectionVisualizerSystem();

	virtual void initialize();
	virtual void process();

	void addEffect(ConnectionEffectData p_fx);
	void disableAll();
protected:
private:
	vector<ConnectionEffectData> m_effectsToCreate;
	vector<ConnectionEffectData> m_createdEffects;

	Entity* createConnectionEffectEntity(ConnectionEffectData& p_data);
};