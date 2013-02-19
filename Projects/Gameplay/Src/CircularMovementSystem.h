#pragma once
#include <AglVector3.h>
#include <EntitySystem.h>
// =======================================================================================
// CircularMovementSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # CircularMovementSystem
/// Detailed description...
/// Created on: 11-2-2013 
///---------------------------------------------------------------------------------------
class CircularMovementSystem: public EntitySystem
{
public:
	CircularMovementSystem();
	~CircularMovementSystem();
	virtual void initialize() final;
	virtual void processEntities(const vector<Entity*>& p_entities) final;
	virtual void sysDisabled() final;

private:
	void initInstanceSphereByJohan(string p_meshName, AglVector3 p_origin,
		AglVector3 p_axis, float p_radius, unsigned int p_numberInstances);

	// Currently unused.
	void initInstanceFieldsByJohan(string p_meshName, unsigned int p_sizeX,
		unsigned int p_sizeY, float p_z, float p_scale);
	void initInstanceAsteroidFieldByJohan(string p_meshName, AglVector3 p_origin, unsigned int p_width,
		unsigned int p_numbersInCircle, float p_radius, float p_spacing, float p_diffY, float p_scale);

};