#include "ClientInfo.h"

ClientInfo::ClientInfo( int p_id )
	: Component( ComponentType::ClientInfo )
{
	id		= p_id;
	ping	= -1;
	score	= 0;
}
