#pragma once

#include "ModuleEvent.h"
#include <Component.h>
#include <ComponentFactory.h>
#include <DamageAccumulator.h>
#include <vector>
using namespace std;

// =======================================================================================
//	Module
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Describes a module that can be attached to a ship
///        
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class ShipModule: public Component
{
public:
	ShipModule();
	ShipModule(float p_maxhealth, float p_maxvalue);
	~ShipModule();
	void init( vector<ComponentData> p_initData );

	/// Add damage for accumulation. The perpetrator id is needed.
	void addDamageThisTick(float p_amount,int p_perpClientId);
	void applyDamage();
	void resetDamage();

	/// Get the id of the latest client to cause damage to this module. \return int The id.
	int getLatestPerpetratorClient();
	bool damageTaken() const;
	const bool& getActive() const;
	void activate();
	void deactivate();
	void addActivationEvent(ModuleEvent* p_event);
	float getMaxHealth();
	float getMaxValue();
	void setToUsed(); ///< Tell module that it has been used
	bool isUnused(); ///< Has the module been used?
	bool isOwned(); ///< Is the module attached to a ship?
	const DamageAccumulator& getDamage();

public:
	int m_parentEntity;
	int m_lastShipEntityWhenAttached;
	float m_value; ///< Value of the module. Generates score.
	float m_health; ///< Health of the module. A module that loses all health is removed

	float m_rotation; ///< Current Rotation of the module
	int	  m_rotationDirection; ///< Current Rotation direction of the module {-1 = Negative, 0 = None, 1 = Positive }

private:
	float m_maxhealth; ///< Max health for this module
	float m_maxvalue; ///< Max value
	bool m_unused; ///< Module is in pristine condition and has never been used.
	bool m_active;
	DamageAccumulator m_damageAcc;
	static ComponentRegister<ShipModule> s_reg;
	vector<ModuleEvent*> m_activationEvents;
};