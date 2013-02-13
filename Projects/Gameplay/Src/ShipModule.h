#pragma once

#include <Component.h>
#include <ComponentFactory.h>
#include "ModuleEvent.h"
#include <vector>
using namespace std;

// =======================================================================================
//	Module
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Describes a module that can be attached to a ship
///        
/// # PlayerScore
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class ShipModule: public Component
{
public:
	int m_parentEntity;
	int m_lastParentWhenAttached;
	float m_value; ///< Value of the module. Generates score.
	float m_health; ///< Health of the module. A module that loses all health is removed

	float m_rotation; ///< Current Rotation of the module
	int	  m_rotationDirection; ///< Current Rotation direction of the module {-1 = Negative, 0 = None, 1 = Positive }

	ShipModule();
	~ShipModule();
	void init( vector<ComponentData> p_initData );
	void addDamageThisTick(float p_amount);
	void applyDamage();
	void resetDamage();
	bool damageTaken() const;
	const bool& getActive() const;
	void activate();
	void deactivate();
	void addActivationEvent(ModuleEvent* p_event);
private:
	bool m_active;
	float m_addedDamage;
	static ComponentRegister<ShipModule> s_reg;
	vector<ModuleEvent*> m_activationEvents;
};