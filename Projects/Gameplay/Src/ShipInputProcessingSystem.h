#pragma once

#include <EntitySystem.h>
#include "ShipInputProcessingSystem.h"
#include "HudSystem.h"

class Control;
class InputActionsBackendSystem;
class InputBackendSystem;

// =======================================================================================
//                              ShipSystemsInputHelper
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Input help structs and methods for the ship, collected in one place
///        
/// # ShipSystemsInputHelper
/// Detailed description.....
/// Created on: 9-1-2013 
///---------------------------------------------------------------------------------------

class ShipInputProcessingSystem : public EntitySystem
{
public:
	ShipInputProcessingSystem(InputBackendSystem* p_inputBackend);
	virtual ~ShipInputProcessingSystem() {}

	virtual void initialize();
	virtual void process();

	struct RawInputForces
	{
		RawInputForces()
		{
			hPositive=0.0f;
			hNegative=0.0f;
			vPositive=0.0f;
			vNegative=0.0f;
			shPositive=0.0f;
			shNegative=0.0f;
			svPositive=0.0f;
			svNegative=0.0f;
			ehPositive=0.0f;
			ehNegative=0.0f;
			evPositive=0.0f;
			evNegative=0.0f;
			rRight=0.0f;
			rLeft=0.0f;
			thrust=0.0f;
			toggleEditMode=false;
		}
		double	hPositive,hNegative,
			vPositive,vNegative,
			shPositive,shNegative,
			svPositive,svNegative,
			ehPositive,ehNegative,
			evPositive,evNegative,
			rRight, rLeft,
			thrust;
		bool toggleEditMode;

		float getHorizontalInput()
		{
			return float(hPositive - hNegative);
		}
		float getVerticalInput()
		{
			return (float)(vPositive - vNegative);
		}
		float getRollInput()
		{
			return (float)(rLeft - rRight);
		}
		float getThrust()
		{
			return (float)(thrust);
		}
		float getStrafeHorizontalInput()
		{
			return (float)(shPositive - shNegative);
		}
		float getStrafeVerticalInput()
		{
			return (float)(svPositive - svNegative);
		}
		float getEditMoveHorizontalInput()
		{
			return (float)(ehPositive - ehNegative);
		}
		float getEditMoveVerticalInput()
		{
			return (float)(evPositive - evNegative);
		}
		bool getStateModeSwitch()
		{
			return toggleEditMode;
		}
	};

	struct ResultingInputForces
	{
		// Store raw float data
		float horizontalInput,
			verticalInput, 
			rollInput,
			thrustInput,
			strafeHorizontalInput,
			strafeVerticalInput,
			editMoveHorizontalInput,
			editMoveVerticalInput;
		bool stateSwitchInput;

		ResultingInputForces() {}

		ResultingInputForces(RawInputForces p_rawInput)
		{
			horizontalInput = p_rawInput.getHorizontalInput();
			verticalInput	= p_rawInput.getVerticalInput();
			rollInput	= p_rawInput.getRollInput();
			thrustInput = p_rawInput.getThrust();
			strafeHorizontalInput	= p_rawInput.getStrafeHorizontalInput();
			strafeVerticalInput		= p_rawInput.getStrafeVerticalInput();
			editMoveHorizontalInput	= p_rawInput.getEditMoveHorizontalInput();
			editMoveVerticalInput	= p_rawInput.getEditMoveVerticalInput();
			stateSwitchInput = p_rawInput.getStateModeSwitch();
		}
	};

	ResultingInputForces& getProcessedInput();

	void setCursorVisibility(bool p_show);
	void resetCursor();

private:
	static const string keymap[];
	static const string gamepadmap[];

	float* getControllerEpsilonPointer();
	RawInputForces readAllInput();
	void updateHudKeymappings(bool p_useGamepad=false);

	ResultingInputForces m_processedInput;

	// Threshold value for the Gamepad's analogue stick error.
	float m_controllerEpsilon;

	float m_angleInputMultiplier;

	bool m_editSwitchTrigReleased;
	InputActionsBackendSystem* m_actionBackend;
	InputBackendSystem* m_inputBackend;
	HudSystem* m_hudSystem;
	bool m_hudMappingsInited;
	bool m_prevDeviceWasGamepad;
};