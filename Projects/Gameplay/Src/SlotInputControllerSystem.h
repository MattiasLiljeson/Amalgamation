#pragma once
#include <EntitySystem.h>
class Control;
class InputBackendSystem;
class TcpClient;
// =======================================================================================
// SlotInputControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Handle the input for slot highlighting and activation.
///        
/// # SlotInputControllerSystem
/// Detailed description...
/// Created on: 4-2-2013 
///---------------------------------------------------------------------------------------
class SlotInputControllerSystem: public EntitySystem
{
public:
	SlotInputControllerSystem(InputBackendSystem* p_inputBackend, TcpClient* p_client);
	~SlotInputControllerSystem();

private:
	void initialize();
	void process();
	void initKeyboard();
	void initGamepad();
	void initMouse();
	void sendModuleSlotHighlight(int p_slot);
	void sendSlotActivation();
	void sendSlotDeactivation();

private:
	InputBackendSystem* m_inputBackend;
	TcpClient* m_client;
	Control* m_keyboardModuleSlots[4];
	Control* m_gamepadModuleSlots[4];
	Control* m_mouseModuleActivation;
	Control* m_gamepadModuleActivation;
};