// =================================================================================================
//                                         AglWriter
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Writes an AglScene to a file.
///
/// 
/// # AglWriter
/// Detailed description will then start here, describe everything
/// using lots of fancy words.
/// Edited By: Anton Andersson, 2012-11-16
///-------------------------------------------------------------------------------------------------

#ifndef AGLWRITER_H
#define AGLWRITER_H

#include "AglMesh.h"
#include "AglSkeleton.h"
#include "AglSkeletonMapping.h"
#include "AglNodeAnimation.h"
#include "AglAnimationLayer.h"
#include "AglAnimation.h"
#include <fstream>
#include <vector>
#include "AglScene.h"

class AglWriter
{
private:
	string m_path;			///< Path to load AGL file from
	AglTopHeader m_header;	///< Top header for the AGL format

private:
	///
	/// Writes vertices to a file
	/// \param p_mesh The mesh to write
	/// \param p_file The file to write to
	///
	void writeVertices(AglMesh* p_mesh, ofstream& p_file);
public:
	///
	/// Constructor
	/// \param p_path The path to write to.
	///
	AglWriter(string p_path);

	///
	/// Writes a scene to the path contained in the writer.
	/// \param p_scene The scene to write.
	///
	void write(AglScene* p_scene);
};

#endif