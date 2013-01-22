#pragma once

#include <Component.h>
#include <ComponentFactory.h>

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

class LoadMesh : public Component
{
public:
	LoadMesh();
	LoadMesh(const string& p_filename);

	virtual void init( vector<ComponentData> p_initData );
	const string& getFileName() {return m_filename;}
protected:
private:
	static ComponentRegister<LoadMesh> s_reg;
	string m_filename;
};