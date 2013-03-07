#pragma once
#include <EntitySystem.h>
#include <AglVector3.h>
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
	void animateHits(int p_fromEntity, int* p_identitiesHit, int p_numberOfHits);

private:
	void animateHit(Entity* p_fromEntity, int p_toEntity, const AglVector3 p_geometricMean);
	void animate(const AglVector3& p_sourcePosition, const AglVector3& p_targetPosition,
		const AglVector3 p_geometricMean);
	bool entityInSystem(Entity* p_checkEntity) const;

};