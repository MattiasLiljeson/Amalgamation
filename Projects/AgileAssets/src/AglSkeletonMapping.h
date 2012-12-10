#ifndef AGLSKELETONMAPPING_H
#define AGLSKELETONMAPPING_H

#include "Agl.h"

// =================================================================================================
//                                         AglSkeletonMappingHeader
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Header of an AglSkeletonMapping. Specifies constant and fixed size data.
///
/// 
/// # AglSkeletonMappingHeader
/// Edited By: Anton Andersson, 2012-11-16
///-------------------------------------------------------------------------------------------------
struct AglSkeletonMappingHeader
{
	int	meshID;			///< Target Mesh ID
	int	skeletonID;		///< Target Skeleton ID
	int mappingType;	///< Type of mapping. Unused
	int	vertexCount;	///< The number of targeted vertices
};

// =================================================================================================
//                                         AglSkeletonMappingVertex
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Vertex of an AglSkeletonMapping. Specifies vertex structure
///
/// 
/// # AglSkeletonMappingVertex
/// Edited By: Anton Andersson, 2012-11-16
///-------------------------------------------------------------------------------------------------
struct AglSkeletonMappingVertex
{
	float			weights[8]; ///< Weight of joints specified by indices
	unsigned int	indices[8];	///< Indices to the joints affecting the vertex
};

// =================================================================================================
//                                         AglSkeletonMapping
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Maps a skeleton to a mesh.
///
/// 
/// # AglSkeletonMapping
/// The AglSkeletonMapping maps a skeleton to mesh. This is done
/// using indices. Furthermore the class contains the vertices
/// necessary to render using bones. These vertices contain
/// bone weights and indices.
/// Edited By: Anton Andersson, 2012-11-16
///-------------------------------------------------------------------------------------------------
class AglSkeletonMapping
{
private:
	AglSkeletonMappingHeader	m_header;		///< Header
	AglSkeletonMappingVertex*	m_vertices;		///< Mapping Vertices
public:
	///
	/// Default Constructor
	///
	AglSkeletonMapping();
	///
	/// Constructor
	/// \param p_header Header.
	/// \param p_vertices Mapping Vertices.
	///
	AglSkeletonMapping(AglSkeletonMappingHeader p_header, AglSkeletonMappingVertex* p_vertices);
	///
	/// Destructor
	///
	~AglSkeletonMapping();

	/// Gets the skeleton mapping header
	/// \return The header
	///
	AglSkeletonMappingHeader getHeader();

	/// Gets the skeleton mapping vertices
	/// \return The vertices
	///
	AglSkeletonMappingVertex* getVertices();

};

#endif