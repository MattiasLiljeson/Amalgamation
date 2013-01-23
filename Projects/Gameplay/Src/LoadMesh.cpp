#include "LoadMesh.h"

ComponentRegister<LoadMesh> LoadMesh::s_reg("LoadMesh");

LoadMesh::LoadMesh()
{
	m_filename="";
	m_componentType = ComponentType::ComponentTypeIdx::LoadMesh;
}

LoadMesh::LoadMesh( const string& p_filename )
{
	m_filename=p_filename;
	m_componentType = ComponentType::ComponentTypeIdx::LoadMesh;
}


void LoadMesh::init( vector<ComponentData> p_initData )
{
	char* cStr;
	for( unsigned int i=0; i<p_initData.size(); i++ )
	{
		if( p_initData[i].dataName == "m_filename" )
		{
			p_initData[i].getDataAsCharArray(&cStr);
			m_filename = string(cStr);
		}
	}
}


