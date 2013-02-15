#include "LevelPieceFileMapping.h"
#include <cstdlib>
#include <DebugUtil.h>
#include <ComponentFactory.h>

LevelPieceFileMapping::LevelPieceFileMapping()
{
	// NOTE: This would better be moved to a parser that knows the exact number of model
	// files, or even move it to assemblages!?!
	//m_fileNames.push_back("test_parts_3sphere.agl");
	//m_fileNames.push_back("test_parts_3sphere.agl");
	//m_fileNames.push_back("test_parts_4tunnel.agl");
	//m_fileNames.push_back("test_parts_4tunnel.agl");
	m_unknown = "(UNKNOWN)";

	//m_asdNames.push_back("tunnelServer");
	//m_fileNames.push_back("World_Tunnel_3Way.agl");
	//m_clientAsdNames.push_back("tunnelClient");

	m_asdNames.push_back("rocksServer");
	m_asdNames.push_back("rocksServer");
	m_fileNames.push_back("WorldPrison_FinalWork_9_exporting.agl");
	m_fileNames.push_back("WorldPrison_FinalWork_9_exporting.agl");

	m_clientAsdNames.push_back("rocksClient");
	m_clientAsdNames.push_back("rocksClient");

	// End plug
	m_asdNames.push_back(m_unknown);
	m_fileNames.push_back("P_cube");

	//resetToDefault();
}

LevelPieceFileMapping::~LevelPieceFileMapping()
{

}

void LevelPieceFileMapping::resetToDefault()
{
	LevelPieceFileData data;

	m_weightData.clear();
	for (int i = m_fileData.size(); i >= 0; i--)
	{
		for (int w = 0; w < m_fileData[i].weight; w++)
			m_weightData.push_back(i); // Pushes back the pieceFileId w times.
	}

}

const string& LevelPieceFileMapping::getAssemblageFileName( unsigned int p_id ) const
{
	if (p_id >= 0 && p_id < m_asdNames.size())
		return m_asdNames[p_id];
	else{
		DEBUGPRINT(("LevelPiece Error, could not find the specified assemblage file \n"));
		return m_unknown;
	}
}

const string& LevelPieceFileMapping::getClientAssemblageFileName( unsigned int p_id ) const
{
	if (p_id >= 0 && p_id < m_clientAsdNames.size())
		return m_clientAsdNames[p_id];
	else{
		DEBUGPRINT(("LevelPiece Error, could not find the specified client assemblage file \n"));
		return m_unknown;
	}
}

const string& LevelPieceFileMapping::getModelFileName(unsigned int p_id ) const
{
	if (p_id >= 0 && p_id < m_fileNames.size())
		return m_fileNames[p_id];
	else{
		DEBUGPRINT(("LevelPiece Error, could not find the specified modelfile \n"));
		return m_unknown;
	}
}

int LevelPieceFileMapping::getModelFileCount() const
{
	return m_fileNames.size();
}

int LevelPieceFileMapping::getRandomPieceId() const
{
	return rand() % (m_fileNames.size() - 1);
}

int LevelPieceFileMapping::getEndPlugId() const
{
	return m_fileNames.size() - 1;
}
