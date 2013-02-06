#pragma once
#include <EntitySystem.h>
#include <string>
using namespace std;
// =======================================================================================
// ClientMeasurementSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # ClientMeasurementSystem
/// Detailed description...
/// Created on: 6-2-2013 
///---------------------------------------------------------------------------------------
class ClientMeasurementSystem: public EntitySystem
{
public:
	ClientMeasurementSystem();
	~ClientMeasurementSystem();
	void process();
	void initialize();
	void initMeasuredSystems();
private:
	vector< pair<EntitySystem*, string> > m_measuredSystems;

};