#pragma once

#include <EntitySystem.h>
#include <AglVector3.h>
#include <AglVector2.h>
#include "SlotMarker.h"

class SlotMarkerSystem : public EntitySystem
{
public:
	SlotMarkerSystem();
	~SlotMarkerSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

	void setSlot(int p_index);
private:
	Entity* createShipMarkerEntity(AglVector3 p_position, string p_texture, AglVector2 p_size);
	void set(SlotMarker* p_marker);
	void unset(SlotMarker* p_marker);

private:
	Entity* slots[4];
	int m_current;

	AglVector2 m_shipMarkerSize;
};

