#include "ShipControllerSystem.h"

ShipControllerSystem::ShipControllerSystem( InputBackendSystem* p_inputBackend ) : 
					  EntitySystem( SystemType::ShipControllerSystem, 2,
									ComponentType::ComponentTypeIdx::ShipController,
									ComponentType::ComponentTypeIdx::Transform)
{
	m_inputBackend = p_inputBackend;
}

ShipControllerSystem::~ShipControllerSystem()
{
	m_horizontalPositive	= m_inputBackend->getInputControl("Mouse X positive");
	m_horizontalNegative	= m_inputBackend->getInputControl("Mouse X negative");
	m_verticalPositive		= m_inputBackend->getInputControl("Mouse Y positive");
	m_verticalNegative		= m_inputBackend->getInputControl("Mouse Y negative");

	m_roll		= m_inputBackend->getInputControl("L");
	m_thrust	= m_inputBackend->getInputControl("Space");

	m_strafeHorizontalPositive	= m_inputBackend->getInputControl("L");
	m_strafeHorizontalNegative	= m_inputBackend->getInputControl("L");
	m_strafeVerticalPositive	= m_inputBackend->getInputControl("L");
	m_strafeVerticalNegative	= m_inputBackend->getInputControl("L");
}

void ShipControllerSystem::initialize()
{

}

void ShipControllerSystem::processEntities( const vector<Entity*>& p_entities )
{

}

