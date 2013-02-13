#pragma once
#include "ModuleEvent.h"
#include <Entity.h>
#include <vector>
using namespace std;
// =======================================================================================
// ShieldModuleActivationClient
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # ShieldModuleActivationClient
/// Detailed description...
/// Created on: 13-2-2013 
///---------------------------------------------------------------------------------------
class ShieldModuleActivationClient: public ModuleEvent
{
public:
	ShieldModuleActivationClient(vector<Entity*> p_plateEntities)
		: plateEntities(p_plateEntities)
	{
	}

	~ShieldModuleActivationClient()
	{
	}

	virtual void activate() final
	{
		for(unsigned int i=0; i<plateEntities.size(); i++)
		{
			plateEntities[i]->setEnabled(true);
		}
	}

	virtual void deactivate() final
	{
		for(unsigned int i=0; i<plateEntities.size(); i++)
		{
			plateEntities[i]->setEnabled(false);
		}
	}

private:
	vector<Entity*> plateEntities;

};