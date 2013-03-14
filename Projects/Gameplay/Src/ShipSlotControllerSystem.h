#pragma once
#include <EntitySystem.h>
class Control;
class InputActionsBackendSystem;
class InputBackendSystem;
class TcpClient;
class ParticleSystemsComponent;
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
	void sendModuleSlotHighlightDeactivateAll();
	void handleSlotSelection(bool p_editMode = false);
private:
	void initialize();
	void process();
	void sendModuleSlotHighlight(int p_slot);
	void sendModuleSlotHighlightDeactivate(int p_slot);

	void sendSlotActivation();
	void sendSlotDeactivation();

	void sendSlotRotationAdd();
	void sendSlotRotationSub();
	void sendSlotRotationNone();

	void sendSlot90Sub();
	void sendSlot90Add();

	void sendPreferredSlotToggle();

private:
	InputActionsBackendSystem* m_actionBackend;
	InputBackendSystem* m_inputBackend;
	TcpClient* m_client;

	// client side fix for remembering slot highlighting
	bool m_previousModeWasEditMode;
	int m_previousHighlight;
};