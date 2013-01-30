#pragma once

#include <Component.h>
#include <ComponentFactory.h>

using namespace std;

// =======================================================================================
//                                      LoadMesh
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
	LoadMesh(const string& p_filename,bool p_isPrimitive=false);

	virtual void init( vector<ComponentData> p_initData );
	const string& getFileName() {return m_filename;}
	bool isPrimitive() {return m_isPrimitive;}
protected:
private:
	static ComponentRegister<LoadMesh> s_reg;
	string m_filename;
	bool m_isPrimitive;
};