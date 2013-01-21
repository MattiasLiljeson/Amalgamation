#include "LevelPieceFileMapping.h"
#include <cstdlib>

LevelPieceFileMapping::LevelPieceFileMapping()
{
	// NOTE: This would better be moved to a parser that knows the exact number of model
	// files, or even move it to assemblages!?!
	//m_fileNames.push_back("test_parts_3sphere.agl");
	//m_fileNames.push_back("test_parts_3sphere.agl");
	m_fileNames.push_back("test_parts_4tunnel.agl");
	// End plug
	m_fileNames.push_back("P_cube");
}

LevelPieceFileMapping::~LevelPieceFileMapping()
{

}

const string& LevelPieceFileMapping::getModelFileName( int p_id ) const
{
	if (p_id >= 0 && p_id < m_fileNames.size())
		return m_fileNames[p_id];
	else
		return "";
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
