#pragma once

#include <Component.h>
#include <ComponentFactory.h>

// =======================================================================================
//	MineLayerModule
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Describes a module that deploys mines
///        
/// # PlayerScore
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class MineLayerModule: public Component
{
public:
	MineLayerModule();
	~MineLayerModule();
	void init( vector<ComponentData> p_initData );

public:
	float m_cooldown;
	float m_timeSinceMineSpawnStart;
	static ComponentRegister<MineLayerModule> s_reg;
};