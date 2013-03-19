#pragma once
#include <EntitySystem.h>
#include <map>
using namespace std;
// =======================================================================================
// EntityLoggerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # EntityLoggerSystem
/// Detailed description...
/// Created on: 18-3-2013 
///---------------------------------------------------------------------------------------
class EntityLoggerSystem: public EntitySystem
{
public:
	EntityLoggerSystem();
	~EntityLoggerSystem();
	void added( Entity* p_entity ) final;
	void deleted( Entity* p_entity ) final;

private:
	map<int, char*> m_entities;
};