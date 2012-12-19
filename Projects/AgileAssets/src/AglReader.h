#ifndef AGLREADER_H
#define AGLREADER_H

#include "AglMesh.h"
#include "AglSkeleton.h"
#include "AglSkeletonMapping.h"
#include "AglNodeAnimation.h"
#include "AglAnimationLayer.h"
#include "AglAnimation.h"
#include "AglScene.h"
#include <fstream>
#include <vector>

class AglReader
{
private:
	ifstream		m_file;		///< File
	AglTopHeader	m_topHeader; ///< Agile Header
	AglScene*		m_scene;		///< Scene to load into

private:

	///
	/// Reads the top header from an AGL file
	///
	void				readTopHeader();

	///
	/// Reads all names from an AGL file
	/// \return An array containing all names
	///
	vector<string>		readNameArray();

	///
	/// Reads a mesh from an AGL file
	/// \return The mesh
	///
	AglMesh*			readMesh();

	///
	/// Reads a material from an AGL file
	/// \return The material
	///
	AglMaterial*		readMaterial();

	///
	/// Reads a material mapping from an AGL file
	/// \return The material mapping
	///
	AglMaterialMapping	readMaterialMapping();

	///
	/// Reads a node from an AGL file
	/// \return The node
	///
	AglNode				readNode();

	///
	/// Reads a skeleton from an AGL file
	/// \return The skeleton
	///
	AglSkeleton*		readSkeleton();

	///
	/// Reads a skeleton mapping from an AGL file
	/// \return The skeleton mapping
	///
	AglSkeletonMapping* readSkeletonMapping();

	///
	/// Reads a node animation from an AGL file
	/// \return The node animation
	///
	AglNodeAnimation*	readNodeAnimation();

	///
	/// Reads an animation layer from an AGL file
	/// \return The animation layer
	///
	AglAnimationLayer*	readAnimationLayer();

	///
	/// Reads an animation from an AGL file
	/// \return The animation
	///
	AglAnimation*		readAnimation();

	///-----------------------------------------------------------------------------------
	/// Reads a bsp tree from an AGL file
	/// \return The bsp tree
	///-----------------------------------------------------------------------------------
	AglLooseBspTree*	readBspTree();

	///-----------------------------------------------------------------------------------
	/// Reads a sphere grid from an AGL file
	/// \return The sphere grid
	///-----------------------------------------------------------------------------------
	AglInteriorSphereGrid* readSphereGrid();

	///
	/// Reads a vertex array from an AGL file
	/// \return An array of a certain vertex type
	///
	template <class T>
	T* readVertices(int p_count);

public:
	

	///
	/// Constructor
	/// \param p_path The path to read from
	///
	AglReader(const char* p_path);

	///
	/// Destructor
	///
	~AglReader();

	///
	/// Get the loaded scene.
	/// \return The scene
	///
	AglScene*					getScene();

};

#endif