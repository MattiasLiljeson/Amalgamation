#include "ComponentData.h"
#include <stdlib.h>

ComponentData::ComponentData()
{
	data = NULL;
	release();
}

void ComponentData::release()
{
	type = AssemblageHelper::AssemblageDataTypes_NON_EXISTING;
	dataName = "";
	dataSize = -1;

	delete data;
	data = NULL;
}

AssemblageHelper::E_FileStatus ComponentData::setData( deque<string> p_elementList )
{
	AssemblageHelper::E_FileStatus status = AssemblageHelper::FileStatus_OK;

	// Make sure no memory leaks 
	release();

	// Read line from stream
	type = AssemblageHelper::typeFromString( p_elementList.front() );
	p_elementList.pop_front();

	dataName = p_elementList.front();
	p_elementList.pop_front();

	switch( type )
	{
	case AssemblageHelper::AssemblageDataTypes_BOOL:
		setBasicDataTypes<bool>( p_elementList );
	case AssemblageHelper::AssemblageDataTypes_INT:
		setBasicDataTypes<int>( p_elementList );
		break;
	case AssemblageHelper::AssemblageDataTypes_UINT:
		setBasicDataTypes<unsigned int>( p_elementList );
		break;
	case AssemblageHelper::AssemblageDataTypes_FLOAT:
		setBasicDataTypes<float>( p_elementList );
		break;
	case AssemblageHelper::AssemblageDataTypes_DOUBLE:
		setBasicDataTypes<double>( p_elementList );
		break;
	case AssemblageHelper::AssemblageDataTypes_CHAR:
		setBasicDataTypes<char>( p_elementList );
		break;
	case AssemblageHelper::AssemblageDataTypes_STRING:
		setDataAsString( p_elementList );
		break;
	case AssemblageHelper::AssemblageDataTypes_VEC2:
		setDataAsVec2( p_elementList );
		break;
	case AssemblageHelper::AssemblageDataTypes_VEC3:
		setDataAsVec3( p_elementList );
		break;
	case AssemblageHelper::AssemblageDataTypes_VEC4:
		setDataAsVec4( p_elementList );
		break;
	default:
		status = AssemblageHelper::FileStatus_COMPONENT_DATA_TYPE_NOT_SUPPORTED;
		break;
	}
	return status;
}


AssemblageHelper::E_FileStatus ComponentData::setDataAsString( deque<string> p_elementList )
{
	AssemblageHelper::E_FileStatus status = AssemblageHelper::FileStatus_COMPONENT_DATA_NOT_SPECIFIED;
	string dataString = "";
	string tempString = "";

	stringstream ss;
	for( unsigned int i=0; i<p_elementList.size(); i++ ) {
		status = AssemblageHelper::FileStatus_OK;
		dataString +=  p_elementList[i];
		if( i < p_elementList.size()-1 ) {
			dataString += ' ';
		}
	}


	setDataAsCharArray( dataString.c_str(), dataString.length()+1 );
	return status;
}

AssemblageHelper::E_FileStatus ComponentData::getDataAsString( string* out_data )
{
	char* cStr;
	getDataAsCharArray( (char**)&cStr );

	if(out_data != NULL )
	{
		*out_data = string(cStr);
		return AssemblageHelper::FileStatus_OK;
	}
	else
	{
		return AssemblageHelper::FileStatus_COMPONENT_DATA_CONVERSION_FAILED;
	}
}

void ComponentData::setDataAsCharArray( const char* p_src, int p_stringLength )
{
	// void* = 4 bytes. char = 1 byte.
	dataSize = ceil( p_stringLength / (float)4 );
	data = new void*[dataSize];
	memcpy( data, p_src, dataSize * sizeof(void*) );
}

void ComponentData::getDataAsCharArray( char** out_data )
{
	*out_data = reinterpret_cast<char*>( data );
}

AssemblageHelper::E_FileStatus ComponentData::setDataAsVec2( deque<string> p_elementList )
{
	if( p_elementList.size() < 2 ) {
		return AssemblageHelper::FileStatus_COMPONENT_DATA_NOT_SPECIFIED;
	}

	float x = atof( p_elementList[0].c_str() );
	float y = atof( p_elementList[1].c_str() );

	// 2*4 = 8 = size of 2 floats.
	data = new void*[8];
	memcpy( data, &x, 4 );
	memcpy( data+4, &y, 4 );

	return AssemblageHelper::FileStatus_OK;
}

AssemblageHelper::E_FileStatus ComponentData::setDataAsVec3( deque<string> p_elementList )
{
	if( p_elementList.size() < 3 ) {
		return AssemblageHelper::FileStatus_COMPONENT_DATA_NOT_SPECIFIED;
	}

	float x = atof( p_elementList[0].c_str() );
	float y = atof( p_elementList[1].c_str() );
	float z = atof( p_elementList[2].c_str() );

	// 3*4 = 12 = size of 3 floats.
	data = new void*[12];
	memcpy( data, &x, 4 );
	memcpy( data+4, &y, 4 );
	memcpy( data+8, &z, 4 );

	return AssemblageHelper::FileStatus_OK;
}

AssemblageHelper::E_FileStatus ComponentData::setDataAsVec4( deque<string> p_elementList )
{
	if( p_elementList.size() < 4 ) {
		return AssemblageHelper::FileStatus_COMPONENT_DATA_NOT_SPECIFIED;
	}

	float x = atof( p_elementList[0].c_str() );
	float y = atof( p_elementList[1].c_str() );
	float z = atof( p_elementList[2].c_str() );
	float w = atof( p_elementList[3].c_str() );

	// 4*4 = 16 = size of 4 floats.
	data = new void*[16];
	memcpy( data, &x, 4 );
	memcpy( data+4, &y, 4 );
	memcpy( data+8, &z, 4 );
	memcpy( data+12, &w, 4 );

	return AssemblageHelper::FileStatus_OK;
}

AssemblageHelper::E_FileStatus ComponentData::getDataAsVec2( float* out_x, float* out_y )
{
	AssemblageHelper::E_FileStatus status = AssemblageHelper::FileStatus_OK;
	if( out_x != NULL && out_y != NULL ) {
		memcpy( out_x, data, 4 );
		memcpy( out_y, data+4, 4 );
	} else {
		status = AssemblageHelper::FileStatus_COMPONENT_DATA_CONVERSION_FAILED;
	}
	return status;
}

AssemblageHelper::E_FileStatus ComponentData::getDataAsVec3( float* out_x, float* out_y, float* out_z )
{
	AssemblageHelper::E_FileStatus status = AssemblageHelper::FileStatus_OK;
	if( out_x != NULL && out_y != NULL && out_z != NULL ) {
		memcpy( out_x, data, 4 );
		memcpy( out_y, data+4, 4 );
		memcpy( out_z, data+8, 4 );
	} else {
		status = AssemblageHelper::FileStatus_COMPONENT_DATA_CONVERSION_FAILED;
	}
	return status;
}

AssemblageHelper::E_FileStatus ComponentData::getDataAsVec4( float* out_x, float* out_y, float* out_z, float* out_w )
{
	AssemblageHelper::E_FileStatus status = AssemblageHelper::FileStatus_OK;
	if( out_x != NULL && out_y != NULL && out_z != NULL && out_w != NULL ) {
		memcpy( out_x, data, 4 );
		memcpy( out_y, data+4, 4 );
		memcpy( out_y, data+8, 4 );
		memcpy( out_y, data+12, 4 );
	} else {
		status = AssemblageHelper::FileStatus_COMPONENT_DATA_CONVERSION_FAILED;
	}
	return status;
}

void ComponentData::operator>>( bool& p_rhs )
{
	getBasicDataTypes<bool>( &p_rhs );
}

void ComponentData::operator>>( int& p_rhs )
{
	getBasicDataTypes<int>( &p_rhs );
}

void ComponentData::operator>>( unsigned int& p_rhs )
{
	getBasicDataTypes<unsigned int>( &p_rhs );
}

void ComponentData::operator>>( float& p_rhs )
{
	getBasicDataTypes<float>( &p_rhs );
}

void ComponentData::operator>>( double& p_rhs )
{
	getBasicDataTypes<double>( &p_rhs );
}

void ComponentData::operator>>( char& p_rhs )
{
	getBasicDataTypes<char>( &p_rhs );

}

void ComponentData::operator>>( string& p_rhs )
{
	getDataAsString( &p_rhs );
}
