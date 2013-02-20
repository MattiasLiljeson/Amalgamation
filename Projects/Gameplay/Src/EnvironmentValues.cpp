#include "EnvironmentValues.h"


ComponentRegister<EnvironmentValues> EnvironmentValues::s_reg("EnvironmentValues");

EnvironmentValues::EnvironmentValues() 
	: Component( ComponentType::EnvironmentValues )
{
	m_fogNearPlaneClosenessPercentage=1.0f;
	m_fogFarPlaneClosenessPercentage=1.0f;
	m_minRadius=500.0f; // The awesomest value!
	m_maxRadius=600.0f; // Unlike this value, it's the worst.
}

EnvironmentValues::~EnvironmentValues()
{

}

void EnvironmentValues::init( vector<ComponentData> p_initData )
{
	for( unsigned int i=0; i<p_initData.size(); i++ )
	{
		string data = p_initData[i].dataName;
		if( data == "m_fogColorR"){
			p_initData[i].getData<float>( &m_fogColor.x );
		} else if( data == "m_fogColorG"){
			p_initData[i].getData<float>( &m_fogColor.y );
		} else if( data== "m_fogColorB"){
			p_initData[i].getData<float>( &m_fogColor.z );
		} else 	if( data == "m_ambientColorR"){
			p_initData[i].getData<float>( &m_ambientColor.x );
		} else if( data == "m_ambientColorG"){
			p_initData[i].getData<float>( &m_ambientColor.y );
		} else if( data== "m_ambientColorB"){
			p_initData[i].getData<float>( &m_ambientColor.z );
		} else if( data == "m_fogFarPlaneClosenessPercentage"){
			p_initData[i].getData<float>( &m_fogFarPlaneClosenessPercentage );
		} else if( data== "m_fogNearPlaneClosenessPercentage"){
			p_initData[i].getData<float>( &m_fogNearPlaneClosenessPercentage);
		} else if( data== "m_minRadius"){
			p_initData[i].getData<float>( &m_minRadius);
		} else if( data== "m_maxRadius"){
			p_initData[i].getData<float>( &m_maxRadius);
		} 
	}
}
