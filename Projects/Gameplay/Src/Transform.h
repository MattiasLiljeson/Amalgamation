// =======================================================================================
//                                      Transform
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Transform Component
///        
/// # Transform
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <AglMatrix.h>
#include <AglVector3.h>
#include <AglQuaternion.h>
#include <Component.h>

#include <InstanceData.h>

class Transform: public Component
{
public:
	///-----------------------------------------------------------------------------------
	/// Sets all private members to 0 (zero).
	/// \return 
	///-----------------------------------------------------------------------------------
	Transform();

	///-----------------------------------------------------------------------------------
	/// Sets the position. Remaining members are set to 0 (zero).
	/// \param p_posX
	/// \param p_posY
	/// \param p_posZ
	/// \return 
	///-----------------------------------------------------------------------------------
	Transform( float p_posX, float p_posY, float p_posZ );
	
	///-----------------------------------------------------------------------------------
	/// creates a transform with position, scale and rotation
	/// \param p_translation
	/// \param p_rotation
	/// \param p_scale
	/// \return 
	///-----------------------------------------------------------------------------------
	Transform(const AglVector3& p_translation, const AglQuaternion& p_rotation, 
		const AglVector3& p_scale);

	Transform(const AglMatrix& p_matrix);

	~Transform();


	///-----------------------------------------------------------------------------------
	/// \return The translation vector.
	///-----------------------------------------------------------------------------------
	const AglVector3& getTranslation() const;
	
	///-----------------------------------------------------------------------------------
	/// \param p_translation The new translation as a vector
	/// \return void
	///-----------------------------------------------------------------------------------
	void setTranslation( const AglVector3& p_translation );

	///-----------------------------------------------------------------------------------
	/// \return The scale vector.
	///-----------------------------------------------------------------------------------
	const AglVector3& getScale() const;

	///-----------------------------------------------------------------------------------
	/// \param p_scale The new scale as a vector
	/// \return void
	///-----------------------------------------------------------------------------------
	void setScale( const AglVector3& p_scale );

	///-----------------------------------------------------------------------------------
	/// \return The rotation quaternion.
	///-----------------------------------------------------------------------------------
	const AglQuaternion& getRotation() const;

	///-----------------------------------------------------------------------------------
	/// \param p_rotation The new rotation as a quaternion
	/// \return void
	///-----------------------------------------------------------------------------------
	void setRotation( const AglQuaternion& p_rotation );

	///-----------------------------------------------------------------------------------
	/// Set forward direction of matrix, doesn't affect scale.
	/// \param p_forward
	/// \return void
	///-----------------------------------------------------------------------------------
	void setForwardDirection( const AglVector3& p_forward );

	///-----------------------------------------------------------------------------------
	/// Set right direction of matrix, doesn't affect scale.
	/// \param p_right
	/// \return void
	///-----------------------------------------------------------------------------------
	void setRightDirection( const AglVector3& p_right );

	///-----------------------------------------------------------------------------------
	/// Set up direction of matrix, doesn't affect scale.
	/// \param p_up
	/// \return void
	///-----------------------------------------------------------------------------------
	void setUpDirection( const AglVector3& p_up );

	///-----------------------------------------------------------------------------------
	/// Set the matrix and update vectors
	/// \param p_matrix
	/// \return void
	///-----------------------------------------------------------------------------------
	void setMatrix(const AglMatrix& p_matrix);

	///-----------------------------------------------------------------------------------
	/// Getter that fetches the pre-calculated matrix that is the sum of all vectors 
	/// \return A pointer to the transform matrix.
	///----------------------------------------------------------------------------------
	const AglMatrix& getMatrix() const;


	inline AglVector3 getForward() const
	{
		return m_compositionMatrix.GetForward();
	}

	inline AglVector3 getRight() const
	{
		return m_compositionMatrix.GetRight();
	}

	inline AglVector3 getUp() const
	{
		return m_compositionMatrix.GetUp();
	}


	///-----------------------------------------------------------------------------------
	/// Get the translated transform matrix packaged in a InstanceVertex.
	/// \return The translated transform matrix as a InstanceVertex.
	///----------------------------------------------------------------------------------
	InstanceData getInstanceVertex() const;

	///-----------------------------------------------------------------------------------
	/// Get a reference to the translated transform matrix packaged in a InstanceVertex.
	/// If the data is manipulated it will change the internal InstanceVertex. This will 
	/// be overwritten the next time a setter is called.
	/// \return A reference to the translated transform matrix as a InstanceVertex.
	///----------------------------------------------------------------------------------
	InstanceData& getInstanceDataRef() ;

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the translated transform matrix packaged in a InstanceVertex.
	/// If the data is manipulated it will change the internal InstanceVertex. This will 
	/// be overwritten the next time a setter is called.
	/// \return A pointer to the translated transform matrix as a InstanceVertex.
	///----------------------------------------------------------------------------------
	InstanceData* getInstanceDataPtr() ;

private:
	///-----------------------------------------------------------------------------------
	/// Calculates the composited matrix for all translation members
	/// \return void
	///-----------------------------------------------------------------------------------
	void calcCompMatrix();

	///-----------------------------------------------------------------------------------
	/// Calculate components from matrix
	/// \return void
	///-----------------------------------------------------------------------------------
	void calcComponents(bool p_calcScale=true, bool p_calcRotation=true, 
						bool p_calcTranslation=true);

private:
	AglMatrix m_compositionMatrix;
	InstanceData m_instanceData;
	//AglMatrix m_transposedCompositionMatrix;
	AglVector3 m_translation;
	AglVector3 m_scale;
	AglQuaternion m_rotation;
};