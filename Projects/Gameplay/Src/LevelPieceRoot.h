#pragma once

#include <Component.h>
#include <AglMatrix.h>
#include <ComponentFactory.h>
#include <AglBoundingSphere.h>

enum ChamberSide
{
	CHAMBERSIDE_PARENT,
	CHAMBERSIDE_CHILD
};

enum PieceRootType
{
	PIECEROOTTYE_CHAMBER,
	PIECEROOTTYE_GATE
};

// =======================================================================================
//                                      LevelPieceRoot
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # LevelPieceRoot
/// Detailed description.....
/// Created on: 14-2-2013 
///---------------------------------------------------------------------------------------

class LevelPieceRoot : public Component
{
public:
	LevelPieceRoot();
	
	int				pieceId;
	PieceRootType	pieceRootType;

	AglBoundingSphere boundingSphere;
	bool shouldCull;

	// This vector is used differently, depending on what the level piece root is representing.
	// The piece root can be either, a chamber piece, or a gate piece.
	// For a chamber, connectedRootPieces represent the pieceIds of gates it's connected to.
	// For a gate, connectedRootPieces represent the two chambers it's connected to. To
	// know what chamber that is on either side on the gate, use the ChamerSide enum.
	vector<int> connectedRootPieces;
private:
	static ComponentRegister<LevelPieceRoot> s_reg;
};