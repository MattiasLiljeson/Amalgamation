#pragma once

#include "IEntityObserver.h"
#include "Entity.h"


class IPerformer
{
public:
	virtual void perform( IEntityObserver* p_observer, Entity* p_entity ) = 0;
};

class AddedPerformer : public IPerformer
{
	void perform( IEntityObserver* p_observer, Entity* p_entity )
	{
		p_observer->added( p_entity );
	}
};

class ChangedPerformer : public IPerformer
{
	void perform( IEntityObserver* p_observer, Entity* p_entity )
	{
		p_observer->changed( p_entity );
	}
};

class DisabledPerformer : public IPerformer
{
	void perform( IEntityObserver* p_observer, Entity* p_entity )
	{
		p_observer->disabled( p_entity );
	}
};

class EnabledPerformer : public IPerformer
{
	void perform( IEntityObserver* p_observer, Entity* p_entity )
	{
		p_observer->enabled( p_entity );
	}
};
class DeletedPerformer : public IPerformer
{
	void perform( IEntityObserver* p_observer, Entity* p_entity )
	{
		p_observer->deleted( p_entity );
	}
};