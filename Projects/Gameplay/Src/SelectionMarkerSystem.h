#pragma once

#include <EntitySystem.h>
#include <AglVector3.h>

class SelectionMarkerSystem : public EntitySystem
{
public:
	SelectionMarkerSystem();
	~SelectionMarkerSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

	void setMarkerTarget(int p_target, AglMatrix p_transform);

private:
	Entity* m_marker;
};