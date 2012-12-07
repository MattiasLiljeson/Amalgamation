// =======================================================================================
//                                      UniqueIndexList
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A list that reuses its indices
///        
/// # UniqueIndexList
/// Detailed description.....
/// Created on: 5-12-2012 
///---------------------------------------------------------------------------------------
#pragma once
#include <stack>
#include <vector>

using namespace std;

template<class T>
class UniqueIndexList
{
public:
	unsigned int add(T p_valueRef);

	bool		 removeAt(unsigned int p_index);

	T			 at(unsigned int p_index);
	T			 operator[](unsigned int p_index);

	void		clear();
protected:
private:
	vector<T> m_list;
	stack<unsigned int> m_freeIndices;
};

template<class T>
void UniqueIndexList<T>::clear()
{
	m_list.clear();
	while(!m_freeIndices.empty())
		m_freeIndices.pop();
}

template<class T>
unsigned int UniqueIndexList<T>::add(T p_valueRef)
{
	unsigned int index = -1;
	if (m_freeIndices.size()>0)
	{
		index = m_freeIndices.top();
		m_list[index] = p_valueRef;
		m_freeIndices.pop();
	}
	else
	{
		m_list.push_back(p_valueRef);
		index = m_list.size()-1;
	}
	return index;
}

template<class T>
bool UniqueIndexList<T>::removeAt(unsigned int p_index)
{
	if (p_index<m_list.size())
	{
		m_list[p_index] = NULL;
		m_freeIndices.push(p_index);
		return true;
	}
	return false;
}

template<class T>
T UniqueIndexList<T>::at(unsigned int p_index)
{
	return m_list[p_index];
}

template<class T>
T UniqueIndexList<T>::operator[](unsigned int p_index)
{
	return m_list[p_index];
}