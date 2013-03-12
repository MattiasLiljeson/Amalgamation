#include "MenuItem.h"

MenuItem::MenuItem()
	: Component( ComponentType::MenuItem )
{
	documentName	= "";
	modal			= false;
	initiallyVisible= false;
}

MenuItem::~MenuItem()
{
}
