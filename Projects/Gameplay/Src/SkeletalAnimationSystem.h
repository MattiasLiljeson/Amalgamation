#pragma once
#include <EntitySystem.h>


// =======================================================================================
//                                      SkeletalAnimationSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # SkeletalAnimationSystem
/// Detailed description.....
/// Created on: 13-02-2013 
///---------------------------------------------------------------------------------------

class SkeletalAnimationSystem : public EntitySystem
{
public:
	SkeletalAnimationSystem();
	~SkeletalAnimationSystem();

	void processEntities( const vector<Entity*>& p_entities );
};