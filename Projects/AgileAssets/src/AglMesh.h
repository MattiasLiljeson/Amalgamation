#ifndef AGLMESH_H
#define AGLMESH_H

#include "Agl.h"
#include "AglBoundingSphere.h"
#include "AglOBB.h"

// =================================================================================================
//                                         AglMeshHeader
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Header of a mesh. Specifies constant and fixed size data.
///
/// 
/// # AglMeshHeader
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
struct AglMeshHeader
{
	int					id;				///< Mesh ID
	int					nameID;			///< Index to the name of the mesh
	AglVertexFormat		vertexFormat;	///< VertexFormat identifier
	int					vertexCount;	///< Number of vertices in the mesh
	int					indexCount;		///< Number of indices in the mesh
	AglBoundingSphere	boundingSphere; ///< Bounding Sphere of the mesh
	AglOBB				minimumOBB;		///< Bounding obb of the mesh

};

// =================================================================================================
//                                         AglVertexSPNT
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Specifies a vertex structure used for handling non-normal mapped meshes.
///
/// 
/// # AglVertexSPNT
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
struct AglVertexSPNT
{
	AglVector3 position;	///< Position of the vertex
	AglVector3 normal;		///< Normal of the vertex
	AglVector2 texCoord;	///< Texture coordinates of the vertex
};

// =================================================================================================
//                                         AglVertexSTBN
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Specifies a vertex structure used for handling normal mapped meshes.
///
/// 
/// # AglVertexSTBN
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
struct AglVertexSTBN
{
	AglVector3 position;	///< Position of the vertex
	AglVector3 normal;		///< Normal of the vertex
	AglVector2 texCoord;	///< Texture coordinates of the vertex
	AglVector3 tangent;		///< Tangent of the vertex
	AglVector3 binormal;	///< Bitangent of the vertex
};

#define AGL_VERTEX_FORMAT_PNT 0
#define AGL_VERTEX_FORMAT_PNTTBN 1

// =================================================================================================
//                                         AglMesh
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Specifies a mesh using vertices and indices
///
/// 
/// # AglMesh
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
class AglMesh
{
private:
	AglMeshHeader	m_header;	///< Mesh header
	void*			m_vertices;  ///< The vertices. Could be of various types
	unsigned int*	m_indices;	///< Indices into the vertex array
public:
	///
	/// Constructor
	/// \param p_header Mesh Header
	/// \param p_vertices List of all vertices in the mesh
	/// \param p_indices List of all indices in the mesh
	///
	AglMesh(AglMeshHeader p_header, void* p_vertices, unsigned int* p_indices);

	///
	/// Destructor
	///
	~AglMesh();

	///
	/// Gets the Mesh header
	/// \return The mesh header
	///
	AglMeshHeader getHeader();

	///
	/// Gets the Mesh vertices
	/// \return The mesh vertices
	///
	void*			getVertices();

	///
	/// Gets the Mesh indices
	/// \return The mesh indices
	///
	unsigned int* getIndices();

	///
	/// Changes the mesh name to the name specified at index p_index
	/// \param p_index The new index to the mesh name
	///
	void setNameIndex(int p_index);

	void transform(AglMatrix p_transform);

};

#endif