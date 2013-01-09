#include "ShipEditModeSystem.h"

ShipEditControllerSystem::ShipEditControllerSystem( InputBackendSystem* p_inputBackend, 
									   PhysicsSystem* p_physicsSystem/*, 
									   TcpClient* p_client */ ) : 
					EntitySystem( SystemType::ShipEditControllerSystem, 2,
								  ComponentType::ComponentTypeIdx::ShipEditController,
								  ComponentType::ComponentTypeIdx::TAG_ShipEditMode,
								  ComponentType::ComponentTypeIdx::Transform)

{

}

ShipEditControllerSystem::~ShipEditControllerSystem()
{

}

void ShipEditControllerSystem::initialize()
{

}

void ShipEditControllerSystem::processEntities( const vector<Entity*>& p_entities )
{

}
