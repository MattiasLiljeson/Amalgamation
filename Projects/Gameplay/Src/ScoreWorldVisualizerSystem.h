#pragma once

#include <Entity.h>
#include <EntitySystem.h>
#include <AglMatrix.h>

using namespace std;
// =======================================================================================
//                              ScoreWorldVisualizerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ScoreWorldVisualizerSystem
/// Detailed description.....
/// Created on: 13-2-2013 
///---------------------------------------------------------------------------------------

class ScoreWorldVisualizerSystem : public EntitySystem
{
public:
	struct ScoreEffectCreationData
	{
		AglMatrix transform;
		int score;
	};


	ScoreWorldVisualizerSystem();
	virtual ~ScoreWorldVisualizerSystem();

	virtual void initialize();
	virtual void process();

	void addEffect(ScoreEffectCreationData& p_fx);
protected:
private:
	vector<ScoreEffectCreationData> m_effectsToCreate;

	Entity* createNumberEffectEntity(ScoreEffectCreationData& p_data);
};