#pragma once
#include <EntitySystem.h>
#include <AglVector3.h>
// =======================================================================================
// MenuBackgroundSceneSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # MenuBackgroundSceneSystem
/// Detailed description...
/// Created on: 19-2-2013 
///---------------------------------------------------------------------------------------
class MenuBackgroundSceneSystem: public EntitySystem
{
public:
	MenuBackgroundSceneSystem();
	~MenuBackgroundSceneSystem();
	virtual void initialize() final;
	virtual void sysDisabled() final;

private:
	void initInstanceSphereByJohan(string p_meshName, AglVector3 p_origin,
		AglVector3 p_axis, float p_radius, unsigned int p_numberInstances);

private:
	Entity* m_ship;
	vector<Entity*> m_rocks;
};