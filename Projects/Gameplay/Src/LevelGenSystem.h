#pragma once

#include <EntitySystem.h>

class Transform;

using namespace std;
// =======================================================================================
//                                      LevelGenSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # LevelGenSystem
/// Detailed description.....
/// Created on: 3-1-2013 
///---------------------------------------------------------------------------------------

class LevelGenSystem : public EntitySystem
{
public:
	LevelGenSystem();
	virtual ~LevelGenSystem();

	void initialize();
protected:

private:
	void getFreeAttachments(Transform* p_piece, vector<Transform*>& out_attachments);

	void generateLevelPieces(int p_maxDepth);
	void generatePiecesOnPiece(Transform* p_targetPiece, vector<Transform*>& out_result);
	void connectPieces(Transform* p_sourcePiece, Transform* p_sourceConnector,
						Transform* p_otherPiece, Transform* p_otherConnector);

};
