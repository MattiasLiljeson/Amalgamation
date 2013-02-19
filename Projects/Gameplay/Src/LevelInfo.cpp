#include "LevelInfo.h"
#include "LevelPieceFileMapping.h"
#include <random>

ComponentRegister<LevelInfo> LevelInfo::s_reg("LevelInfo");

LevelInfo::LevelInfo()
	: Component(ComponentType::LevelInfo)
{

}

LevelInfo::~LevelInfo()
{
	for (unsigned int i = 0; i < m_fileData.size(); i++)
	{
		delete m_fileData[i];
	}
	m_fileData.clear();
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
		else if (p_initData[i].dataName == "m_plugPiece")
		{
			p_initData[i].getDataAsString(&m_plugPiece);
		}
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
	return m_fileData[p_id];
}

LevelPieceFileData* LevelInfo::getStartFileData() const
{
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



