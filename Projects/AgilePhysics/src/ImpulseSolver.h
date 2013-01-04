#ifndef IMPULSESOLVER_H
#define IMPULSESOLVER_H

#include "CollisionManager.h"

///-----------------------------------------------------------------------------------
/// Desc
/// \param p_collisionData Data relevant to resolve the collision
/// \param p_dt Timestep used at the time of collision
/// \param p_cor Coefficient of resistution [0, 1]
/// \return Always returns true
///-----------------------------------------------------------------------------------
bool ResolveCollision(PhyCollisionData* p_collisionData, float p_dt, float p_cor);

#endif