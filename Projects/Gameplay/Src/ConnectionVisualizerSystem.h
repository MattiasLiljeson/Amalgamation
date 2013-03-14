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
		AglVector3	forward;
		Entity*		data;
		bool		disabled;

		ConnectionEffectData(Entity* p_parent, int p_slot, AglVector3 p_offset, AglVector3 p_forward, bool p_disabled)
		{
			parent = p_parent;
			slot = p_slot;
			offset = p_offset;
			forward = p_forward;
			disabled = p_disabled;
		}
	};


	ConnectionVisualizerSystem();
	virtual ~ConnectionVisualizerSystem();

	virtual void initialize();
	virtual void process();

	void addEffect(ConnectionEffectData p_fx);
	void disableAll();

	void cleanup();
protected:
private:
	vector<ConnectionEffectData> m_effectsToCreate;
	vector<ConnectionEffectData> m_createdEffects;

	Entity* createConnectionEffectEntity(ConnectionEffectData& p_data);
	void	replaceConnectionEffectEntity(ConnectionEffectData& p_data, int p_index);
};