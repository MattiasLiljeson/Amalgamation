#include "LevelInfo.h"
#include "LevelPieceFileData.h"
#include <random>
#include <sstream>

ComponentRegister<LevelInfo> LevelInfo::s_reg("LevelInfo");

LevelInfo::LevelInfo()
	: Component(ComponentType::LevelInfo)
{
	m_branchCount			= -1;
	m_startPieceId			= -1;
	m_randomStartRotation	= false;
	m_endPlugOpened			= NULL;
	m_endPlugClosed			= NULL;

	memset(m_levelSize, 0, sizeof(m_levelSize));
}

LevelInfo::~LevelInfo()
{
	delete m_endPlugOpened;
	delete m_endPlugClosed;
	m_endPlugOpened = NULL;
	m_endPlugClosed = NULL;
	for (unsigned int i = 0; i < m_fileData.size(); i++)
	{
		delete m_fileData[i];
	}
	m_fileData.clear();
	memset(m_levelSize, 0, sizeof(m_levelSize));
}

void LevelInfo::init( vector<ComponentData> p_initData )
{

	LevelPieceFileData* pieceData = NULL;

	for (unsigned int i = 0; i < p_initData.size(); i++)
	{
		if (p_initData[i].dataName == "m_randomStartRotation")
		{
			int b;
			p_initData[i].getData<int>(&b);
			m_randomStartRotation = static_cast<bool>(b);
		}
		else if (p_initData[i].dataName == "m_branchCount")
		{
			p_initData[i].getData<int>(&m_branchCount);
		}
		else if (p_initData[i].dataName == "piece")
		{
			string data;
			p_initData[i].getDataAsString(&data);
			if (data == "begin")
				pieceData = new LevelPieceFileData();
			else // If not begin, then the data is assumed to be "end"
			{
				pieceData->id = m_fileData.size();
				m_fileData.push_back(pieceData);
				addWeightData(pieceData);
			}
		}
		else if (p_initData[i].dataName == "m_pieceAssemblageFileName")
		{
			p_initData[i].getDataAsString(&pieceData->assemblageFileName);
		}
		else if (p_initData[i].dataName == "m_pieceModelName")
		{
			p_initData[i].getDataAsString(&pieceData->modelFileName);
		}
		else if (p_initData[i].dataName == "m_pieceWeight")
		{
			p_initData[i].getData<int>(&pieceData->weight);
		}
		else if (p_initData[i].dataName == "m_startPieceId")
		{
			p_initData[i].getData<int>(&m_startPieceId);
		}
		else if (p_initData[i].dataName == "m_plugAssemblageOpened")
		{
			m_endPlugOpened = new LevelPieceFileData();
			p_initData[i].getDataAsString(&m_endPlugOpened->assemblageFileName);
		}
		else if (p_initData[i].dataName == "m_plugModelOpened")
		{
			p_initData[i].getDataAsString(&m_endPlugOpened->modelFileName);
		}
		else if (p_initData[i].dataName == "m_plugAssemblageClosed")
		{
			m_endPlugClosed = new LevelPieceFileData();
			p_initData[i].getDataAsString(&m_endPlugClosed->assemblageFileName);
		}
		else if (p_initData[i].dataName == "m_plugModelClosed")
		{
			p_initData[i].getDataAsString(&m_endPlugClosed->modelFileName);
		}
		else if (p_initData[i].dataName == "m_levelSize")
		{
			string data;
			p_initData[i].getDataAsString(&data);
			setLevelSize(data);
		}
	}
	if (m_endPlugOpened)
	{
		m_endPlugOpened->id		= ENDPIECEMODE_OPENED;
		m_endPlugOpened->weight	= 0;
	}
	if (m_endPlugClosed)
	{
		m_endPlugClosed->id		= ENDPIECEMODE_CLOSED;
		m_endPlugClosed->weight	= 0;
	}
}

const vector<LevelPieceFileData*>& LevelInfo::getFileData() const
{
	return m_fileData;
}

LevelPieceFileData* LevelInfo::getRandomFileData() const
{
	return m_fileData[ m_weightData[ rand() % m_weightData.size() ] ];
}

LevelPieceFileData* LevelInfo::getFileDataFromId( int p_id ) const
{
	if (p_id < m_fileData.size())
		return m_fileData[p_id];
	else
		return NULL;
}

LevelPieceFileData* LevelInfo::getStartFileData() const
{
	if (m_startPieceId == -1)
		return getRandomFileData();
	return m_fileData[m_startPieceId];
}

int LevelInfo::getBranchCount() const
{
	return m_branchCount;
}

void LevelInfo::addWeightData( LevelPieceFileData* p_fromFileData )
{
	for (int w = 0; w < p_fromFileData->weight; w++)
		m_weightData.push_back(p_fromFileData->id); // Pushes back the pieceFileId w times.
}

bool LevelInfo::doRandomStartRotation() const
{
	return m_randomStartRotation;
}

LevelPieceFileData* LevelInfo::getEndPlugFileData(EndPieceMode p_mode) const
{
	if (p_mode == ENDPIECEMODE_OPENED)
		return m_endPlugOpened;
	else
		return m_endPlugClosed;
}

void LevelInfo::setLevelSize(string p_fromData )
{
	int nrOfPlayers, size;
	char waste;
	stringstream stream(p_fromData);
	stream >> nrOfPlayers >> waste >> size;

	m_levelSize[nrOfPlayers-1] = size;
}

int LevelInfo::getLevelSize( int p_playerCount ) const
{
	return m_levelSize[p_playerCount-1];
}

