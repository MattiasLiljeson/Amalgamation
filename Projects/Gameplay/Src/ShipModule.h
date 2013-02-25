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
	int m_lastShipEntityWhenAttached;
	float m_value; ///< Value of the module. Generates score.
	float m_health; ///< Health of the module. A module that loses all health is removed

	float m_rotation; ///< Current Rotation of the module
	int	  m_rotationDirection; ///< Current Rotation direction of the module {-1 = Negative, 0 = None, 1 = Positive }

	ShipModule();
	~ShipModule();
	void init( vector<ComponentData> p_initData );
	///-----------------------------------------------------------------------------------
	/// Add damage for accumulation
	/// The perpetrator id is needed.
	/// \param p_amount
	/// \param p_perpEntityShipId
	/// \return void
	///-----------------------------------------------------------------------------------
	void addDamageThisTick(float p_amount,int p_perpClientId);
	void applyDamage();
	void resetDamage();
	///-----------------------------------------------------------------------------------
	/// Get the id of the latest client to cause damage to this module
	/// \return int
	///-----------------------------------------------------------------------------------
	int getLatestPerpetratorClient();
	bool damageTaken() const;
	const bool& getActive() const;
	void activate();
	void deactivate();
	void addActivationEvent(ModuleEvent* p_event);
	void setToUsed(); ///< Tell module that it has been used
	bool isUnused(); ///< Has the module been used?
private:
	struct DamageAccumulator
	{
		DamageAccumulator(){reset();latestPerp=-1;}
		void reset() {accumulatedDamage=0.0f;}
		float accumulatedDamage;
		int latestPerp;
	};
	bool m_unused; ///< Module is in pristine condition and has never been used.
	bool m_active;
	DamageAccumulator m_damageAcc;
	static ComponentRegister<ShipModule> s_reg;
	vector<ModuleEvent*> m_activationEvents;
};