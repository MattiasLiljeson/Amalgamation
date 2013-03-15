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
	void addMarker(int p_slot, EntityType p_type);
	void removeMarker(int p_slot, EntityType p_type);
	void clear();
private:
	Entity* createShipMarkerEntity(AglVector3 p_position, string p_texture, AglVector2 p_size);
	Entity* createModuleMarkerEntity(AglVector3 p_position, string p_texture, AglVector2 p_size);
	void set(SlotMarker* p_marker);
	void unset(SlotMarker* p_marker);
	void arrangeChildren(SlotMarker* p_marker, bool p_marked);
	void reinit(int p_slot);

private:
	Entity* slots[4];
	AglVector3 positions[4];
	int m_current;

	AglVector2 m_shipMarkerSize;
	AglVector2 m_moduleMarkerSize;

	pair<int, string> m_textures[10];

	AglVector2 m_screenSize;
};

