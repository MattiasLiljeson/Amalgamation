#include "ShieldModule.h"

ShieldModule::ShieldModule()
	: Component( ComponentType::ShieldModule )
{
	m_shieldEntity = -1;
	m_shieldAge = 0;
	m_cooldown = 0;
}
ShieldModule::~ShieldModule()
{

}