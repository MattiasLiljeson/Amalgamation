#pragma once

//#include "AnalogueControl.h"
#include "Control.h"
//#include "DigitalControl.h"
#include "DirectInputFetcher.h"
//#include "InputType.h"
#include "MessageLoopFetcher.h"
#include "XInputFetcher.h"

#include <vector>

//class AnalogueControl;
class Control;
//class DigitalControl;
class DirectInputFetcher;
//class InputType;
class MessageLoopFetcher;
class XInputFetcher;

using namespace std;

class InputManager
{
public:
	/// Std Constructor. Takes ownership of the supplied Fetchers and therefore deletes
	/// them in its destructor.
	///\param p_mlFetcher The Message Loop fetcher to be used by the manager.
	///\param p_xiFetcher The xInput fetcher to be used by the manager.
	InputManager( MessageLoopFetcher* p_mlFetcher, XInputFetcher* p_xiFetcher );
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
	MessageLoopFetcher* getMessageLoopFetcher() const;
	XInputFetcher* getXInputFetcher() const;
	void update();

private:
	static const int DIGITAL_ID_OFFSET = INT_MAX/2;
	vector<Control*> m_controls;

	MessageLoopFetcher* m_mlFetcher;
	XInputFetcher* m_xiFetcher;
};

