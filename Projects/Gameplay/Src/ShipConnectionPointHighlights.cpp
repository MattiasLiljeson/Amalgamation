#include "ShipConnectionPointHighlights.h"


ShipConnectionPointHighlights::ShipConnectionPointHighlights() 
	: Component( ComponentType::ShipConnectionPointHighlights)
{
	for (unsigned int i=0;i<slots;i++)
	{
		slotStatus[i]=false;
	}
}
