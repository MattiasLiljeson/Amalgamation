#include "LevelInfoLoader.h"
#include "LevelInfo.h"
#include "LevelPieceFileData.h"
#include "EntityFactory.h"
#include <string>
#include <ToString.h>
#include <OutputLogger.h>

using namespace std;

LevelInfoLoader::LevelInfoLoader()
	: EntitySystem(SystemType::LevelInfoLoader)
{
	m_levelInfo = NULL;
}

LevelInfoLoader::~LevelInfoLoader()
{

}

void LevelInfoLoader::initialize()
{
	auto entityFactory = static_cast<EntityFactory*>(
		m_world->getSystem(SystemType::EntityFactory));


	string recipeName;
	entityFactory->readAssemblageFile(LEVELPIECESPATH + "LevelGenClient.asd", &recipeName);


	Entity* e = entityFactory->entityFromRecipe(recipeName);


	m_levelInfo = static_cast<LevelInfo*>(e->getComponent(ComponentType::LevelInfo));
	
	vector<LevelPieceFileData*> fileData = m_levelInfo->getFileData();

	for (int i = 0; i < fileData.size(); i++)
	{
		//m_world->getOutputLogger()->write( ("LevelInfoLoader begun loading" + fileData[i]->assemblageFileName + "\n").c_str() );

		// Preload the chamber assemblages.
		entityFactory->readAssemblageFile(LEVELPIECESPATH + fileData[i]->assemblageFileName,
			&fileData[i]->assemblageName);

		//m_world->getOutputLogger()->write( ("LevelInfoLoader finished loading" + fileData[i]->assemblageFileName + "\n").c_str() );
	}

	LevelPieceFileData* endPlug = m_levelInfo->getEndPlugFileData(ENDPIECEMODE_OPENED);
	//m_world->getOutputLogger()->write( ("LevelInfoLoader begun loading" + endPlug->assemblageFileName + "\n").c_str() );
	entityFactory->readAssemblageFile(LEVELPIECESPATH + endPlug->assemblageFileName,
		&endPlug->assemblageName);
	//m_world->getOutputLogger()->write( ("LevelInfoLoader finished loading" + endPlug->assemblageFileName + "\n").c_str() );

	endPlug = m_levelInfo->getEndPlugFileData(ENDPIECEMODE_CLOSED);
	//m_world->getOutputLogger()->write( ("LevelInfoLoader begun loading" + endPlug->assemblageFileName + "\n").c_str() );
	entityFactory->readAssemblageFile(LEVELPIECESPATH + endPlug->assemblageFileName,
		&endPlug->assemblageName);
	//m_world->getOutputLogger()->write( ("LevelInfoLoader finished loading" + endPlug->assemblageFileName + "\n").c_str() );

	m_world->addEntity(e); 
}

void LevelInfoLoader::removed( Entity* p_entity )
{
	m_levelInfo = NULL;
}

LevelPieceFileData* LevelInfoLoader::getFileData( int p_id, StaticPropType p_type ) const
{
	if (m_levelInfo)
	{
		if (p_type == STATICPROPTYPE_LEVELPIECE)
			return m_levelInfo->getFileDataFromId(p_id);
		else if (p_type == STATICPROPTYPE_PLUGPIECE)
			return m_levelInfo->getEndPlugFileData((EndPieceMode)p_id);
		else
			return NULL;
	}
	else
		return NULL;
}
