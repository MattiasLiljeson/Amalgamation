#pragma once
#include <EntitySystem.h>
#include <AglVector3.h>
class InputActionsBackendSystem;
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
	virtual void process() final;
	virtual void initialize() final;
	virtual void sysEnabled() final;
	virtual void sysDisabled() final;
	virtual void inserted( Entity* p_entity ) final;

private:
	void initInstanceSphereByJohan(string p_meshName, AglVector3 p_origin,
		AglVector3 p_axis, float p_radius, unsigned int p_numberInstances);
	void initRandomModulesSphere(AglVector3 p_origin, AglVector3 p_axis, float p_radius,
		unsigned int p_numberInstances);
	void initPointLight(Entity* p_entity, AglVector3 p_position, float p_range);
	void initOrbitingShip(AglVector3 p_center, AglVector3 p_axis, float p_radius,
		 float p_speed);
	void initLogo(AglVector3 p_center, float p_size);

private:
	AglVector3 m_center;
	//Entity* m_ship;
	Entity* m_orbitingShip;
	Entity* m_logo;
	Entity* m_chamber;
	Entity* m_mainCamera;
	bool m_logoInit;

	vector<Entity*> m_rocks;
	vector<Entity*> m_modules;
	vector<Entity*> m_lights;
	InputActionsBackendSystem* m_actionBackend;
	float m_deltaRotation;
	float xPos;
};