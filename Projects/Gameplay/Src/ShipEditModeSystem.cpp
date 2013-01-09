#include "ShipEditModeSystem.h"

ShipEditControllerSystem::ShipEditControllerSystem( InputBackendSystem* p_inputBackend, 
									   PhysicsSystem* p_physicsSystem/*, 
									   TcpClient* p_client */ ) : 
					EntitySystem( SystemType::ShipFlyControllerSystem, 2,
								  ComponentType::ComponentTypeIdx::ShipEditController,
								  ComponentType::ComponentTypeIdx::ShipEditMode_TAG,
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
