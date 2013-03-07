#pragma once
#include <EntitySystem.h>
#include <AglVector3.h>
#include <map>
#include <vector>
using namespace std;
// =======================================================================================
// TeslaEffectSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Animates a lighting strike from one entity to another.
/// 
/// # TeslaEffectSystem
/// Created on: 2-3-2013 
///---------------------------------------------------------------------------------------
class TeslaEffectSystem: public EntitySystem
{
public:
	TeslaEffectSystem();
	void inserted( Entity* p_entity ) final;
	void animateHits(int p_fromEntity, int* p_identitiesHit, int p_numberOfHits,
		int* p_identitiesHitFloating, int p_numberOfHitsFloating);

private:
	void animateHit(Entity* p_fromEntity, int p_toEntity, const AglVector3 p_geometricMean,
		int p_index, bool p_damage);
//	void animate(const AglVector3& p_sourcePosition, const AglVector3& p_targetPosition,
//		const AglVector3 p_geometricMean, string p_meshName);
	bool entityInSystem(Entity* p_checkEntity) const;

private:
	map<int, vector< vector<Entity*> > > m_lightningPool;

};