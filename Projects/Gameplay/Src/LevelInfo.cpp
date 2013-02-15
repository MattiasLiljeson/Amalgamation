#include "LevelInfo.h"
#include "LevelPieceFileMapping.h"

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
		else if (p_initData[i].dataName == "begin_piece")
		{
			pieceData = new LevelPieceFileData();
		}
		else if (p_initData[i].dataName == "m_pieceAssemblageName")
		{
			p_initData[i].getDataAsString(&pieceData->assemblageName);
		}
		else if (p_initData[i].dataName == "m_pieceModelName")
		{
			p_initData[i].getDataAsString(&pieceData->modelFileName);
		}
		else if (p_initData[i].dataName == "m_pieceWeight")
		{
			p_initData[i].getData<int>(&pieceData->weight);
		}
	}
}
