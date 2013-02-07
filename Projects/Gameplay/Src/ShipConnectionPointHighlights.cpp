#include "ShipConnectionPointHighlights.h"


ShipConnectionPointHighlights::ShipConnectionPointHighlights() 
	: Component( ComponentType::ShipConnectionPointHighlights)
{
	current=0;
	for (int i=0;i<4;i++)
	{
		slotStatus[i]=false;
	}
}
