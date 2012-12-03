#include "ServerApplication.h"

ServerApplication::ServerApplication()
{
	m_running = false; // set running to true if initialization is ok!

	m_world = new EntityWorld();

	initSystems();

	m_running = true;
}

ServerApplication::~ServerApplication()
{
	delete m_world; /*
	Detected memory leaks!
	Dumping objects ->
	{168} normal block at 0x00769A08, 8 bytes long.
	 Data: <h v     > 68 99 76 00 00 00 00 00 
	{167} normal block at 0x007699B0, 24 bytes long.
	 Data: <  v   v   v     > B0 99 76 00 B0 99 76 00 B0 99 76 00 01 01 CD CD 
	{166} normal block at 0x00769960, 20 bytes long.
	 Data: <$     v   v   v > 24 B5 1F 00 D0 92 76 00 08 9A 76 00 B0 99 76 00 
	{163} normal block at 0x007698D0, 8 bytes long.
	 Data: <  v     > 90 97 76 00 00 00 00 00 
	{162} normal block at 0x00769888, 8 bytes long.
	 Data: <x v     > 78 97 76 00 00 00 00 00 
	{161} normal block at 0x00769840, 8 bytes long.
	 Data: <| v     > 7C 97 76 00 00 00 00 00 
	{160} normal block at 0x007697F8, 8 bytes long.
	 Data: <h v     > 68 97 76 00 00 00 00 00 
	{159} normal block at 0x00769760, 88 bytes long.
	 Data: <(     v   v     > 28 B0 1F 00 D0 92 76 00 F8 97 76 00 00 00 00 00 
	{156} normal block at 0x00769690, 8 bytes long.
	 Data: <  v     > F8 95 76 00 00 00 00 00 
	{155} normal block at 0x00769648, 8 bytes long.
	 Data: <  v     > E8 95 76 00 00 00 00 00 
	{154} normal block at 0x007695E0, 40 bytes long.
	 Data: <$     v H v     > 24 A9 1F 00 D0 92 76 00 48 96 76 00 00 00 00 00 
	*/
}

void ServerApplication::run()
{
	while( m_running )
	{
		m_world->setDelta( 0.016f );

		m_world->process();
	}
}

void ServerApplication::initSystems()
{
	m_world->setSystem( SystemType::NetworkListenerSystem,
		new NetworkListenerSystem(), true );

	m_world->initialize();

}
