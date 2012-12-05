// =======================================================================================
//                                      ResourceMap
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A map structure for storing and accessing resources by use of a unique name(like 
/// a file name from which the resource was loaded). It prohibits duplicated resources.
///        
/// # ResourceMap
/// Created on: 03-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <map>
#include <string>
#include "DebugUtil.h"

using namespace std;

template <class T>
class ResourceMap
{
public:
		~ResourceMap() {clear();}
		void clear();
		T** getResource(const string& p_uniqueName);
		T** addToMap(const string& p_uniqueName, T* p_resourceObj); ///< Add shader to the map structure for allocation control and removal
		bool removeFromMap(const string& p_uniqueName);
private:
		map<string,T**> m_resourceMap;
};


template <class T>
void ResourceMap<T>::clear()
{
		typename map<string,T**>::iterator mapIter = m_resourceMap.begin();
		for (; mapIter!=m_resourceMap.end(); mapIter++)
		{
				delete *mapIter->second;
				delete mapIter->second;
		}
		m_resourceMap.clear();
};

template <class T>
T** ResourceMap<T>::getResource(const string& p_uniqueName)
{
		T** resourceObj = NULL;
		typename map<string,T**>::iterator mapIter = m_resourceMap.find(uniqueName);

		if(mapIter != m_resourceMap.end()) 
				resourceObj = mapIter->second;

		return resourceObj;
};

template <class T>
T** ResourceMap<T>::addToMap(const string& p_uniqueName, T* p_resourceObj)
{
		// do an insert and return either the inserted value or the already existing value
		T** reval = NULL;
		typename map<string,T**>::iterator mapIter = m_resourceMap.insert(typename map<string,T**>::value_type(uniqueName,reval));
		reval = mapIter->second;	
		
		return reval;
}

template <class T>
bool ResourceMap::removeFromMap(const string& p_uniqueName)
{
	m_resourceMap.erase(p_uniqueName)==1?return true:return false;
}