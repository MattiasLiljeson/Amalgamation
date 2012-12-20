#pragma once
#include <EntitySystem.h>
#include <windows.h>

#include "InputHelper.h"

class inputControlFactory;
class InputManager;
class GraphicsBackendSystem;
class Control;
// =======================================================================================
//                                      InputSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # InputSystem
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------

class InputBackendSystem : public EntitySystem
{
public:

	///-----------------------------------------------------------------------------------
	/// Constructor. HINSTANCE is needed to fetch input from a window. The 
	/// GraphicsBackendSystem is also needed to fetch input from a window.
	/// \param p_hInstance
	/// \param p_graphicsBackend
	/// \return 
	///-----------------------------------------------------------------------------------
	InputBackendSystem( HINSTANCE p_hInstance, GraphicsBackendSystem* p_graphicsBackend );
	~InputBackendSystem();

	virtual void initialize();
	///-----------------------------------------------------------------------------------
	/// Updates the input manager which updates all of its controls.
	/// \return void
	///-----------------------------------------------------------------------------------
	void process();
	
	///-----------------------------------------------------------------------------------
	/// Get a pointer to a Control by supplying a string with the name the Control has
	/// been registered with. O(log(n)).
	/// \param p_name
	/// \return Control*
	///-----------------------------------------------------------------------------------
	Control* getInputControl(const string& p_name);

	///-----------------------------------------------------------------------------------
	/// Get a pointer to a Control by supplying a enum. This function is overridden for
	/// each input device type. O(1).
	/// \param p_key The control registered to the key/btn/whatever you want to get.
	/// \return Control* A ptr to a Control that is updated each frame with fresh input
	///-----------------------------------------------------------------------------------
	Control* getControlByEnum( InputHelper::KEYBOARD_KEY p_key );
	Control* getControlByEnum( InputHelper::MOUSE_BTN p_btn );
	Control* getControlByEnum( InputHelper::MOUSE_AXIS p_axis );
	Control* getControlByEnum( InputHelper::XBOX360_CONTROLLER_DIGITAL p_digital );
	Control* getControlByEnum( InputHelper::XBOX360_CONTROLLER_ANALOG p_analog );

private:
	///-----------------------------------------------------------------------------------
	/// Helper function used to add a Control to all maps. 
	/// \param p_deviceType Input device type. Used when adding the Control to the correct
	/// input map
	/// \param p_controlType The type of control. Used when adding the Comntrol to the
	/// correct index in its map
	/// \param p_control A pointer to the Control to add. Must be placed on the heap as the
	/// maps use the supplied pointer from this function.
	/// \param p_name Name used for debug and when working with the Control. These are
	/// often the same as items in the different enums
	/// \return void
	///-----------------------------------------------------------------------------------
	void saveControl( InputHelper::INPUT_DEVICE_TYPE p_deviceType, int p_controlType,
		Control* p_control, const string& p_name );

	///-----------------------------------------------------------------------------------
	/// Helper function which maps a device type to its corresponding map. 
	/// \param p_deviceType The device type for which you want the corresponding vector
	/// \return vector<int>* a pointer to the vector of the type given above. This vector
	/// is allocated on the stack
	///-----------------------------------------------------------------------------------
	vector<int>* vectorFromDeviceType( InputHelper::INPUT_DEVICE_TYPE p_deviceType );

	///-----------------------------------------------------------------------------------
	/// Helper function. Expand a vector if it's size is too small for the given index.
	/// Uses no member variables, therefore static.
	/// \param p_vec Pointer to a vector which is to be controlled.
	/// \param p_idx The index to control.
	/// \return void
	///-----------------------------------------------------------------------------------
	static void expandIdxVectorIfNecessary( vector<int>* p_vec, int p_idx );

private:
	GraphicsBackendSystem* m_graphicsBackend;
	HINSTANCE m_hInstance;
	InputManager* m_inputManager;

	map<string, int> m_controlIdxs;
	vector<int> m_mouseMoveIdxsbyEnum;
	vector<int> m_mouseBtnIdxsbyEnum;
	vector<int> m_keyIdxsbyEnum;
	vector<int> m_xboxAnalogIdxsbyEnum;
	vector<int> m_xboxDigitalIdxsbyEnum;
};

