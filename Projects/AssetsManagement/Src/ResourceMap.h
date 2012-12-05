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
#include <UniqueIndexList.h>

using namespace std;

template <class T>
class ResourceMap
{
public:
		~ResourceMap() {clear();}

		///-----------------------------------------------------------------------------------
		/// Clear the entire resource map and all its data.
		/// \returns void
		///-----------------------------------------------------------------------------------
		void clear();

		///-----------------------------------------------------------------------------------
		/// Get resource data by its name. Uses a map to access the data.
		/// \param p_uniqueName
		/// \returns T*
		///-----------------------------------------------------------------------------------
		T* getResource(const string& p_uniqueName);

		///-----------------------------------------------------------------------------------
		/// Get resource data by its id. Uses an UniqueIndexList to access the data O(1);
		/// \param p_uniqueId
		/// \returns T*
		///-----------------------------------------------------------------------------------
		T* getResource(unsigned int p_uniqueId);

		///-----------------------------------------------------------------------------------
		/// Get resource data by its id. Uses an UniqueIndexList to access the data O(1);
		/// \param p_uniqueId
		/// \returns T*
		///-----------------------------------------------------------------------------------
		T* operator[](unsigned int p_uniqueId);

		///-----------------------------------------------------------------------------------
		/// Get the id of resource data by its name. Uses a map to access the data.
		/// \param p_uniqueName
		/// \returns unsigned int
		///-----------------------------------------------------------------------------------
		unsigned int getResourceId(const string& p_uniqueName);

		///-----------------------------------------------------------------------------------
		/// Get the name of resource data by its id. 
		/// Uses an UniqueIndexList to access the data O(1).
		/// \param p_uniqueId
		/// \returns const string&
		///-----------------------------------------------------------------------------------
		const string& getResourceName(unsigned int p_uniqueId);

		///-----------------------------------------------------------------------------------
		/// Adds another resource to the internal storage if the unique name does not already
		/// exist. If it does, the index of the existing data is returned.
		/// \param p_uniqueName
		/// \param p_resourceObj
		/// \returns unsigned int
		///-----------------------------------------------------------------------------------
		unsigned int addResource(const string& p_uniqueName, T* p_resourceObj);

		///-----------------------------------------------------------------------------------
		/// Remove a resource by its name. Uses a map to access the data.
		/// \param p_uniqueName
		/// \returns bool
		///-----------------------------------------------------------------------------------
		bool removeResource(const string& p_uniqueName);

		///-----------------------------------------------------------------------------------
		/// Remove a resource by its id. Uses a map to access the data. 
		/// Uses an UniqueIndexList to access the data O(1).
		/// \param p_uniqueId
		/// \returns bool
		///-----------------------------------------------------------------------------------
		bool removeResource(unsigned int p_uniqueId);
private:
		///---------------------------------------------------------------------------------------
		/// \brief	Internal structure used in order to get access to data as well as key and id.
		///        
		/// # ResourceDataContainer
		/// Created on: 05-12-2012 
		///---------------------------------------------------------------------------------------
		struct ResourceDataContainer
		{
			~ResourceDataContainer() {delete data;}
			T* data;
			string uniqueName;
			unsigned int uniqueId;
		};

		///-----------------------------------------------------------------------------------
		/// Help method used in order to get the resource data container from its name.
		/// ResourceDataContainer is only used internally.
		/// \param p_uniqueName
		/// \returns ResourceDataContainer*
		///-----------------------------------------------------------------------------------
		ResourceDataContainer* getResourceContainerFromName(const string& p_uniqueName);

		///
		/// A map of all the resources for access by unique key.
		///
		map<string,ResourceDataContainer*> m_resourceMap;

		///
		/// A list of all the resources for O(1) access.
		///
		UniqueIndexList<ResourceDataContainer*> m_resourceList;
};


template <class T>
void ResourceMap<T>::clear()
{
		typename map<string,T*>::iterator mapIter = m_resourceMap.begin();
		for (; mapIter!=m_resourceMap.end(); mapIter++)
		{
				// delete *mapIter->second;
				delete mapIter->second;
		}
		m_resourceMap.clear();
};

template <class T>
T* ResourceMap<T>::getResource(const string& p_uniqueName)
{
		T* resourceObj = getResourceContainerFromName(p_uniqueName)->data;

		return resourceObj;
};

template <class T>
T* ResourceMap<T>::getResource(unsigned int p_uniqueId)
{
	return m_resourceList[p_uniqueId]->data;
};

template <class T>
T* ResourceMap<T>::operator[](unsigned int p_uniqueId)
{
	return m_resourceList[p_uniqueId]->data;
};

template <class T>
unsigned int ResourceMap<T>::getResourceId(const string& p_uniqueName)
{
	return getResourceContainerFromName(p_uniqueName)->uniqueId;
};

template <class T>
const string& ResourceMap<T>::getResourceName(unsigned int p_uniqueId)
{
	return m_resourceList[p_uniqueId]->uniqueName;
};

template <class T>
unsigned int ResourceMap<T>::addResource(const string& p_uniqueName, T* p_resourceObj)
{
		// do an insert and return either the inserted value or the already existing value
		unsigned int reval = -1;

		// Create empty container
		ResourceMap::ResourceDataContainer* container = new ResourceDataContainer();

		// Try to make an insertion using the unique name as key
		// if it already exists we get a pointer to the existing data
		typename map<string,ResourceDataContainer*>::iterator mapIter;

		mapIter = m_resourceMap.insert(
			typename map<string,ResourceDataContainer*>::value_type(
				uniqueName,container)
			);

		// if new insertion was made,
		// add data and keys to the container
		if (mapIter->second == container)
		{
			container->data = p_resourceObj;
			container->uniqueName = p_uniqueName;
			// add the container to the list as well, return the id
			// and store in container
			container->uniqueId = m_resourceList.add(container); 
		}

		// return existing or inserted container's id member
		reval = mapIter->second->uniqueId;	
		return reval;
}

template <class T>
bool ResourceMap<T>::removeResource(const string& p_uniqueName)
{
	m_resourceMap.erase(p_uniqueName)==1?return true:return false;
}

template <class T>
bool ResourceMap<T>::removeResource(unsigned int p_uniqueId)
{
	return m_resourceList.removeAt(p_uniqueId);
}

template <class T>
typename ResourceMap<T>::ResourceDataContainer* ResourceMap<T>::getResourceContainerFromName(const string& p_uniqueName)
{
	ResourceDataContainer* resourceContainer = NULL;
	typename map<string,ResourceDataContainer*>::iterator mapIter;
	mapIter = m_resourceMap.find(uniqueName);

	if(mapIter != m_resourceMap.end()) 
		resourceObj = mapIter->second;

	return resourceContainer;
}
