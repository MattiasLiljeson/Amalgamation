#pragma once

#include <EntitySystem.h>
#include <AglVector3.h>
#include <AglVector4.h>
#include <AglOBB.h>

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
	bool* getCollidesWithChamber()
	{
		return &m_collidesWithChamber;
	}
	bool* getCollidesWithGate()
	{
		return &m_collidesWithGate;
	}

private:
	void calcCameraPlanes();
	Entity* findRootPieceOrGate(int p_id, const vector<Entity*>& p_list);
	bool frustumGate(Entity* p_gate);
	bool BoxPlane(const AglOBB& p_box, const AglVector4& p_plane);
	bool IsAdded(Entity* p_chamber, const vector<Entity*>& p_list);
	bool CameraPosInGate(Entity* p_gate);
private:
	int m_culledChambers;
	int m_renderedChambers;

	AglVector4 m_cameraPlanes[6];
	AglVector3 m_cameraPos;

	Entity* m_collidedGate;
	Entity* m_collidedChamber;

	bool m_collidesWithGate;
	bool m_collidesWithChamber;
};

