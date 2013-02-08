#pragma once
#include <EntitySystem.h>
#include <vector>
#include <string>
#include <PreciseTimer.h>
using namespace std;
// =======================================================================================
// ServerMeasurementSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # ServerMeasurementSystem
/// Detailed description...
/// Created on: 6-2-2013 
///---------------------------------------------------------------------------------------
class ServerMeasurementSystem: public EntitySystem
{
public:
	ServerMeasurementSystem();
	~ServerMeasurementSystem();
	void process();
	void initialize();

private:
	vector< pair<EntitySystem*, string> > m_measuredSystems;
	PreciseTimer m_secondTimer;

};