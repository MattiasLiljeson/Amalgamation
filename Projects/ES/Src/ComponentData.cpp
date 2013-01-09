#include "ComponentData.h"

void ComponentData::release()
{
	dataType = -1;
	dataName = "";
	dataSize = -1;

	delete data;
	data = NULL;
}