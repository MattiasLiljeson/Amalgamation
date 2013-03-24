#include "EntityLoggerSystem.h"
#include "AntTweakBarWrapper.h"
#include <ToString.h>

EntityLoggerSystem::EntityLoggerSystem()
	: EntitySystem(SystemType::EntityLoggerSystem)
{
}

EntityLoggerSystem::~EntityLoggerSystem()
{
}

void EntityLoggerSystem::added( Entity* p_entity )
{
	if(this->getEnabled())
	{
		AntTweakBarWrapper::getInstance()->addReadOnlyVariable(AntTweakBarWrapper::OVERALL,
			(toString(p_entity->getIndex()) + ": " + p_entity->getName()).c_str(), TwType::TW_TYPE_INT32,
			p_entity->getIndexPtr(), "group='entities'");
	}
}

void EntityLoggerSystem::deleted( Entity* p_entity )
{
	if(this->getEnabled())
	{
		TwRemoveVar(AntTweakBarWrapper::getInstance()->getAntBar(AntTweakBarWrapper::OVERALL),
			(toString(p_entity->getIndex()) + ": " + p_entity->getName()).c_str());
	}
}