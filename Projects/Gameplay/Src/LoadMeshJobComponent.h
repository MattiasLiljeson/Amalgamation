#pragma once

#include <Component.h>

// =======================================================================================
//                                      LoadMeshJobComponent
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A component specifying a string path to a mesh to load
///        
/// # LoadMeshJobComponent
/// Detailed description.....
/// Created on: 22-1-2013 
///---------------------------------------------------------------------------------------

class LoadMeshJobComponent : public Component
{
public:
	LoadMeshJobComponent(const string& p_filename) {m_filename=p_filename;}
	const string& getFileName() {return m_filename;}
protected:
private:
	string m_filename;
};