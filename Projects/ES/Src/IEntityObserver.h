#pragma once

#include "Entity.h"

class Entity;

class IEntityObserver 
{
public:
	virtual void added(Entity* p_entity) = 0;
	virtual void changed(Entity* p_entity) = 0;
	virtual void deleted(Entity* p_entity) = 0;
	virtual void enabled(Entity* p_entity) = 0;
	virtual void disabled(Entity* p_entity) = 0;
};