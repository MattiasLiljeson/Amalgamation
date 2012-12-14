#pragma once


#include "Control.h"
#include "DirectInputFetcher.h"
#include "IMouseKeyboardFetcher.h"
#include "MessageLoopFetcher.h"
#include "XInputFetcher.h"
#include <vector>

class Control;
class DirectInputFetcher;
class MessageLoopFetcher;
class XInputFetcher;

using namespace std;

// =======================================================================================
//                                      InputManager
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Manager class that manages all Controls that are added to it.
///        
/// All Controls added to the manager are deleted by the manager. I.e. the manager takes
/// ownership of the controls.
///
/// Use the Input Control Factory to create Controls and add them to the manager so that
/// they get updated. Then use the index given by the addControl() function to fetch the 
/// Control and its latest status. You can, if you wish keep the pointer to the Control 
/// that you use when adding the Control and use that to fetch status from the Control. 
///
/// Created on: 6-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------

class InputManager
{
public:
	/// Std Constructor. Takes ownership of the supplied Fetchers and therefore deletes
	/// them in its destructor.
	///\param p_mlFetcher The Message Loop fetcher to be used by the manager.
	///\param p_xiFetcher The xInput fetcher to be used by the manager.
	InputManager( IMouseKeyboardFetcher* p_mlFetcher, XInputFetcher* p_xiFetcher );
	~InputManager();

	/// @brief Adds a Control to the manager.
	/// The manager takes ownership of the control.
	/// @return An index used when working with the Control through the handler.
	int addControl( Control* p_control );

	/// Get a control.
	///\param p_idx Index of the control you wish to fetch.
	///\return NULL if the index isn't bound to a control. If the control is found a
	/// pointer to it is returned.
	Control* getControl( int p_idx );

	/// @brief getters used by controls to update themselves
	IMouseKeyboardFetcher* getMouseKeyboardFetcher() const;
	XInputFetcher* getXInputFetcher() const;
	void update();

private:
	static const int DIGITAL_ID_OFFSET = INT_MAX/2;
	vector<Control*> m_controls;

	IMouseKeyboardFetcher* m_mouseKeyboardFetcher;
	XInputFetcher* m_xInputFetcher;

};

