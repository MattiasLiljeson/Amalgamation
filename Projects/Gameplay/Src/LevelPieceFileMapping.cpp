#include "LevelPieceFileMapping.h"
#include <cstdlib>
#include <DebugUtil.h>

LevelPieceFileMapping::LevelPieceFileMapping()
{
	// NOTE: This would better be moved to a parser that knows the exact number of model
	// files, or even move it to assemblages!?!
	//m_fileNames.push_back("test_parts_3sphere.agl");
	//m_fileNames.push_back("test_parts_3sphere.agl");
	//m_fileNames.push_back("test_parts_4tunnel.agl");
	//m_fileNames.push_back("test_parts_4tunnel.agl");
	m_fileNames.push_back("WorldPrison4.agl");
	// End plug
	m_fileNames.push_back("P_cube");

	m_unknown = "(UNKNOWN)";
}

LevelPieceFileMapping::~LevelPieceFileMapping()
{

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
