// =======================================================================================
//                                      ResourceManager
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A generic manager for resources. It prohibits duplicates and can be accessed
/// by index( O(1) ) or map.
///        
/// # ResourceManager
/// Created on: 03-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <map>
#include <string>
#include <UniqueIndexList.h>

using namespace std;

template <class T>
class ResourceManager
{
public:
		~ResourceManager() {clear();}

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
		int getResourceId(const string& p_uniqueName);

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
		typedef struct ResourceDataContainer
		{
			~ResourceDataContainer() {delete data;}
			T* data;
			string uniqueName;
			unsigned int uniqueId;
		} ResourceDataContainer;

		typedef map<string,ResourceDataContainer*> MapType;

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
		MapType m_resourceMap;

		///
		/// A list of all the resources for O(1) access.
		///
		UniqueIndexList<ResourceDataContainer*> m_resourceList;
};


template <class T>
void ResourceManager<T>::clear()
{
		MapType::iterator mapIter = m_resourceMap.begin();
		for (; mapIter!=m_resourceMap.end(); mapIter++)
		{
				// delete *mapIter->second;
				delete mapIter->second;
		}
		m_resourceMap.clear();
};

template <class T>
T* ResourceManager<T>::getResource(const string& p_uniqueName)
{
		T* resourceObj = getResourceContainerFromName(p_uniqueName)->data;

		return resourceObj;
};

template <class T>
T* ResourceManager<T>::getResource(unsigned int p_uniqueId)
{
	return m_resourceList[p_uniqueId]->data;
};

template <class T>
T* ResourceManager<T>::operator[](unsigned int p_uniqueId)
{
	return m_resourceList[p_uniqueId]->data;
};

template <class T>
int ResourceManager<T>::getResourceId(const string& p_uniqueName)
{
	ResourceDataContainer* container = getResourceContainerFromName(p_uniqueName);
	if (container!=NULL)
		return container->uniqueId;
	else
		return -1;
};

template <class T>
const string& ResourceManager<T>::getResourceName(unsigned int p_uniqueId)
{
	return m_resourceList[p_uniqueId]->uniqueName;
};

template <class T>
unsigned int ResourceManager<T>::addResource(const string& p_uniqueName, T* p_resourceObj)
{
		// do an insert and return either the inserted value or the already existing value
		unsigned int reval = -1;

		// Create empty container
		ResourceDataContainer* container = new ResourceDataContainer();

		// Try to make an insertion using the unique name as key
		// if it already exists we get a pointer to the existing data
		MapType::iterator mapIter;
		MapType::iterator mapIterBegin = m_resourceMap.begin();
		mapIter = m_resourceMap.insert(mapIterBegin, 
									   MapType::value_type(p_uniqueName,container));

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
bool ResourceManager<T>::removeResource(const string& p_uniqueName)
{
	m_resourceMap.erase(p_uniqueName)==1?return true:return false;
}

template <class T>
bool ResourceManager<T>::removeResource(unsigned int p_uniqueId)
{
	return m_resourceList.removeAt(p_uniqueId);
}

template <class T>
typename ResourceManager<T>::ResourceDataContainer* ResourceManager<T>::getResourceContainerFromName(const string& p_uniqueName)
{
	ResourceDataContainer* resourceContainer = NULL;
	MapType::iterator mapIter = m_resourceMap.find(p_uniqueName);

	if(mapIter != m_resourceMap.end()) 
		resourceContainer = mapIter->second;

	return resourceContainer;
}
