#include "LightsComponent.h"

#include <LightInstanceData.h>
#include <AglMatrix.h>
#include <algorithm>

ComponentRegister<LightsComponent> LightsComponent::s_reg("LightsComponent");

Light::Light()
{
	offsetMat = AglMatrix::identityMatrix();
	instanceData.shadowIdx = -1;
}


LightsComponent::LightsComponent()
	: Component( ComponentType::LightsComponent )
{
}

LightsComponent::~LightsComponent()
{
}

vector<Light>* LightsComponent::getLightsPtr()
{
	return &m_lights;
}

void LightsComponent::addLight( Light& p_light  )
{
	m_lights.push_back( p_light );
}

void LightsComponent::init( vector<ComponentData> p_initData )
{
	vector<TransformComponents> transforms; 

	for( unsigned int i=0; i<p_initData.size(); i++ )
	{
		// Find the light index and make sure there's room for it
		string idxAsStr = p_initData[i].dataName.substr( 0, 1 );
		unsigned int lightIdx = atoi( idxAsStr.c_str() );

		if( lightIdx >= m_lights.size() ) {
			m_lights.resize( lightIdx+1 );
		}

		if( lightIdx >= transforms.size() ) {
			transforms.resize( lightIdx+1 );		
		}

		// Read data about the particular light
		string lightInfoStr = p_initData[i].dataName.substr( 2 );

		// Basic settings
		if( lightInfoStr == "range"){ 
			p_initData[i].getData<float>( &m_lights[lightIdx].instanceData.range );
		} else if( lightInfoStr == "enabled"){ 
			p_initData[i].getData<int>( &m_lights[lightIdx].instanceData.enabled );
		} else if( lightInfoStr == "energy" || lightInfoStr == "lightEnergy" ) { 
			p_initData[i].getData<float>( &m_lights[lightIdx].instanceData.lightEnergy );
		}
		
		// Spot settings
		else if( lightInfoStr == "spotPower" ||
			lightInfoStr == "spotLightConeSizeAsPow" ||
			lightInfoStr == "spotLightConeSize" ||
			lightInfoStr == "spotLightCone") {
			p_initData[i].getData<float>( &m_lights[lightIdx].instanceData.spotLightConeSizeAsPow );
		} 
		else if( lightInfoStr == "lightDir" ) { 
			float x, y, z;
			p_initData[i].getDataAsVec3( &x, &y, &z );
			m_lights[lightIdx].instanceData.lightDir[0] = x;
			m_lights[lightIdx].instanceData.lightDir[1] = y;
			m_lights[lightIdx].instanceData.lightDir[2] = z;
		}

		// Transformation of the light volumes
		else if( lightInfoStr == "scale"){
						float x, y, z;
			p_initData[i].getDataAsVec3( &x ,&y, &z );
			transforms[lightIdx].scale.x = x;
			transforms[lightIdx].scale.y = y;
			transforms[lightIdx].scale.z = z;
		} 
		else if( lightInfoStr == "rotation"){
			float x, y, z, w;
			p_initData[i].getDataAsVec4( &x ,&y, &z, &w );
			transforms[lightIdx].rotation.u.x = x;
			transforms[lightIdx].rotation.u.y = y;
			transforms[lightIdx].rotation.u.z = z;
			transforms[lightIdx].rotation.v = w;
		} 
		else if( lightInfoStr == "translation"){
			float x, y, z;
			p_initData[i].getDataAsVec3( &x ,&y, &z );
			transforms[lightIdx].translation.x = x;
			transforms[lightIdx].translation.y = y;
			transforms[lightIdx].translation.z = z;
		} 
		
		// Attenuation
		else if( lightInfoStr == "attenuation"){
			float lin, quad, cube;
			p_initData[i].getDataAsVec3( &lin ,&quad ,&cube );
			m_lights[lightIdx].instanceData.attenuation[0] = lin;
			m_lights[lightIdx].instanceData.attenuation[1] = quad;
			m_lights[lightIdx].instanceData.attenuation[2] = cube;
		} 
		
		else if( lightInfoStr == "color"){
			float r, g, b;
			p_initData[i].getDataAsVec3( &r, &g, &b );
			m_lights[lightIdx].instanceData.color[0] = r;
			m_lights[lightIdx].instanceData.color[1] = g;
			m_lights[lightIdx].instanceData.color[2] = b;
		} 
		
		// Light type
		else if( lightInfoStr == "typeAsNum"){ 
			p_initData[i].getData<int>( &m_lights[lightIdx].instanceData.type );
		}

		// special case when type is given as a string and not int
		else if( lightInfoStr == "typeAsStr" || lightInfoStr == "typeAsString" ) 
		{
			string lightType;
			p_initData[i]>>lightType;
			std::transform( lightType.begin(), lightType.end(), lightType.begin(), ::tolower );

			if( lightType == "dir" || 
				lightType == "directional" || 
				lightType == "directionallight" || 
				lightType == "directional_light") 
			{ 
				m_lights[lightIdx].instanceData.type = LightTypes::E_LightTypes_DIRECTIONAL;
			} 
			else if( lightType == "point" || 
				lightType == "pointlight" || 
				lightType == "point_light") 
			{ 
				m_lights[lightIdx].instanceData.type = LightTypes::E_LightTypes_POINT;
			} 
			else if( lightType == "spot" || 
				lightType == "spotlight" || 
				lightType == "spot_light")
			{ 
				m_lights[lightIdx].instanceData.type = LightTypes::E_LightTypes_SPOT;
			}
		}

		else if( lightInfoStr == "hasShadow"){
			p_initData[i].getData<int>( &m_lights[lightIdx].instanceData.shadowIdx );
		}
	}

	for( unsigned int i=0; i<m_lights.size(); i++ )
	{
		m_lights[i].offsetMat = transforms[i].toMatrix();
	}
}
