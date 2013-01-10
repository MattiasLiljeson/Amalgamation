#include "ComponentData.h"

ComponentData::ComponentData()
{
	data = NULL;
	release();
}

void ComponentData::release()
{
	dataType = -1;
	dataName = "";
	dataSize = -1;

	delete data;
	data = NULL;
}