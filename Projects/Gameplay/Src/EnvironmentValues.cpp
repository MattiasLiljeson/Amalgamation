#include "EnvironmentValues.h"


ComponentRegister<EnvironmentValues> EnvironmentValues::s_reg("EnvironmentValues");

EnvironmentValues::EnvironmentValues() 
	: Component( ComponentType::EnvironmentValues )
{
	fogNearPlaneClosenessPercentage=1.0f;
	fogFarPlaneClosenessPercentage=1.0f;
	radius=500.0f; // the awesomest value
}

EnvironmentValues::~EnvironmentValues()
{

}

void EnvironmentValues::init( vector<ComponentData> p_initData )
{
	for( unsigned int i=0; i<p_initData.size(); i++ )
	{
		string data = p_initData[i].dataName;
		if( data == "fogColorR"){
			p_initData[i].getData<float>( &fogColor.x );
		} else if( data == "fogColorG"){
			p_initData[i].getData<float>( &fogColor.y );
		} else if( data== "fogColorB"){
			p_initData[i].getData<float>( &fogColor.z );
		} else 	if( data == "ambientColorR"){
			p_initData[i].getData<float>( &ambientColor.x );
		} else if( data == "ambientColorG"){
			p_initData[i].getData<float>( &ambientColor.y );
		} else if( data== "ambientColorB"){
			p_initData[i].getData<float>( &ambientColor.z );
		} else if( data == "fogFarPlaneClosenessPercentage"){
			p_initData[i].getData<float>( &fogFarPlaneClosenessPercentage );
		} else if( data== "fogNearPlaneClosenessPercentage"){
			p_initData[i].getData<float>( &fogNearPlaneClosenessPercentage);
		} else if( data== "radius"){
			p_initData[i].getData<float>( &radius);
		} 
	}
}
