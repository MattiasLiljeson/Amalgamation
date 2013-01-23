// =======================================================================================
//                                      PhysicsBody
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # PhysicsBody
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------
#pragma once
#include <Component.h>
#include <AglVector3.h>
#include <vector>
#include <ComponentFactory.h>
using namespace std;

class PhysicsBody : public Component
{
public:

	PhysicsBody();

	int m_id;

	virtual void init( vector<ComponentData> p_initData );

	int		getParentId() {return m_parentId;}
	int		getOldParentId() {return m_oldParentId;}

	///-----------------------------------------------------------------------------------
	/// Sets a parent to this body, will only work if parent is compound body.
	/// \param p_parentId
	/// \return void
	///-----------------------------------------------------------------------------------
	void	setParentId(int p_parentId);

	///-----------------------------------------------------------------------------------
	/// Sets compound parent to none.
	/// \return void
	///-----------------------------------------------------------------------------------
	void	unspecifyParent();

	int		isParentChanged() {return m_parentChanged;}
	void	resetParentChangedStatus() {m_parentChanged=false;}

private:
	static ComponentRegister<PhysicsBody> s_reg;

	int m_parentId;
	int m_oldParentId;
	bool m_parentChanged;
};