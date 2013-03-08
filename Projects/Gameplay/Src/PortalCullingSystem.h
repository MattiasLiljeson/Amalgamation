#pragma once

#include <EntitySystem.h>
#include <AglVector3.h>
#include <AglVector4.h>

class PortalCullingSystem : public EntitySystem
{
public:
	PortalCullingSystem();
	virtual ~PortalCullingSystem();
	virtual void initialize();

	virtual void processEntities( const vector<Entity*>& p_entities );

	int* getCulledChambers()
	{
		return &m_culledChambers;
	}
	int* getRenderedChambers()
	{
		return &m_renderedChambers;
	}

private:
	void calcCameraPlanes();
private:
	int m_culledChambers;
	int m_renderedChambers;

	AglVector4 m_cameraPlanes[6];
	AglVector3 m_cameraPos;
};

