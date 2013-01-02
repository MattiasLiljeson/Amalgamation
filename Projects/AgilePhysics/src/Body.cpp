#include "Body.h"

Body::Body()
{
	mActive = true;
}
Body::~Body()
{

}

void Body::Activate()
{
	mActive = true;
}
void Body::Inactivate()
{
	mActive = false;
}
bool Body::IsActive()
{
	return mActive;
}