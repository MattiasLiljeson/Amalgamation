#include "StandardRocket.h"

StandardRocket::StandardRocket()
	: Component( ComponentType::StandardRocket )
{
	m_age = 0;
	m_ownerId = -1;
}
StandardRocket::~StandardRocket()
{

}