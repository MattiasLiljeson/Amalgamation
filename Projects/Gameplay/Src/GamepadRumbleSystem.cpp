#include "GamepadRumbleSystem.h"

#include "Vibration.h"
#include "InputBackendSystem.h"
#include "Transform.h"
#include "NetworkSynced.h"
#include "ValueClamp.h"
#include <AntTweakBarWrapper.h>

GamepadRumbleSystem::GamepadRumbleSystem(InputBackendSystem* p_input)
	: EntitySystem( SystemType::GamepadRumbleSystem, 1, ComponentType::Vibration)
{
	m_input = p_input;
	m_enabled = false;
}

GamepadRumbleSystem::~GamepadRumbleSystem()
{

}



void GamepadRumbleSystem::initialize()
{
	AntTweakBarWrapper::getInstance()->addWriteVariable(AntTweakBarWrapper::OVERALL,
		"rumble", TwType::TW_TYPE_BOOLCPP, &m_enabled, "");
}


void GamepadRumbleSystem::processEntities( const vector<Entity*>& p_entities )
{
	if (m_enabled)
	{
		float dt = m_world->getDelta();
		float left = 0.0f;
		float right = 0.0f;
		int vibrationsL = 0;
		int vibrationsR = 0;

		// get ship
		Transform* ship=NULL;
		Entity* playerShip =
			m_world->getEntityManager()->getFirstEntityByComponentType(
			ComponentType::TAG_MyShip );
		if( playerShip != NULL )
		{
			ship = static_cast<Transform*>( playerShip->getComponent(ComponentType::Transform) );
		}

		// get all vibrations in the air
		for(unsigned int i=0; i<p_entities.size(); i++)
		{
			Vibration* vibration = static_cast<Vibration*>(p_entities[i]->getComponent(
				ComponentType::Vibration));

			if (vibration->enabled)
			{
				// decline effect
				float multiplier=1.0f;
				if (vibration->enableDeclineEffect)
				{
					if (vibration->linearDeclineEffectMultiplier>0.0f)
					{
						vibration->linearDeclineEffectMultiplier-=dt;
						multiplier=vibration->linearDeclineEffectMultiplier;
					}
					else
					{
						multiplier=0.0f;
						vibration->linearDeclineEffectMultiplier = -1.0f;
						vibration->enableDeclineEffect = false;
						vibration->enabled = false;
					}
				}
				float leftL = vibration->leftPower*multiplier;
				float rightL = vibration->rightPower*multiplier;
				// set rumble
				if (vibration->distanceAttenuated) // "3d"
				{
					// get rumble center
					Transform* epicenter=NULL;
					Component* t = p_entities[i]->getComponent( ComponentType::Transform );
					if (t!=NULL)
						epicenter= static_cast<Transform*>(t);

					float att = 1.0f;

					if (epicenter && ship)
					{
						float dist = AglVector3::length(ship->getTranslation()-epicenter->getTranslation());
						// Calculate linear attenuation
						float min = vibration->minRadius;
						float max = vibration->maxRadius;
						att = 1.0f-((clamp(dist, min, max+min)-min)/max);
					}

					leftL *= att;
					rightL *= att;
				}				
				if (leftL>0.001f)
				{
					left += leftL;
					vibrationsL++;
				}
				if (rightL>0.001f)
				{
					right += rightL;
					vibrationsR++;
				}		
			}
		}		
		// calculate total average rumble
		left /= max(1.0f,(float)vibrationsL);
		right /= max(1.0f,(float)vibrationsR);
		if (vibrationsL==0)
			left=0.0f;
		if (vibrationsR==0)
			right=0.0f;
		m_input->setControllerVibration(left,right);

		// send rumble info to server for camera shake etc
		if (right>0.0f || left>0.0f)
		{
			NetworkSynced* netSync = static_cast<NetworkSynced*>(playerShip->getComponent(
				ComponentType::NetworkSynced));
			sendRumblePacketToServer(netSync,left,right);
		}
	}
	else
	{
		m_input->setControllerVibration(0.0f,0.0f);
	}
}
	

void GamepadRumbleSystem::sendRumblePacketToServer(NetworkSynced* p_syncedInfo, 
												   float p_power1, float p_power2)
{
	/* WIP, send rumble data to server here
	ThrustPacket thrustPacket;
	thrustPacket.entityId = p_syncedInfo->getNetworkIdentity();
	thrustPacket.thrustVector = p_thrust;
	thrustPacket.angularVector = p_angularVec;

	m_client->sendPacket( thrustPacket.pack() );*/
}