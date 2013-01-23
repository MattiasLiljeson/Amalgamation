#pragma once

// =======================================================================================
//                            ComponentAssemblageAllocator
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Allocate one of each component that should be read by an assemblage
/// to ensure symbol loading.
///        
/// # ComponentAssemblageAllocator
/// Detailed description.....
/// Created on: 23-1-2013 
///---------------------------------------------------------------------------------------

class ComponentAssemblageAllocator
{
public:
	ComponentAssemblageAllocator();
protected:
private:
	template<class T>
	void handle();
};

template<class T>
void ComponentAssemblageAllocator::handle()
{
	T* tvalue = new T();
	delete tvalue;
}