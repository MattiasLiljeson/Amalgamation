#pragma once

#include <Component.h>
#include <ComponentFactory.h>

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
	bool m_active;
	float m_value; ///< Value of the module. Generates score.
	float m_health; ///< Health of the module. A module that loses all health is removed

	ShipModule();
	~ShipModule();
	void init( vector<ComponentData> p_initData );
	void addDamageThisTick(float p_amount);
	void applyDamage();
	void resetDamage();
	bool damageTaken() const;
private:
	float m_addedDamage;
	static ComponentRegister<ShipModule> s_reg;
};