#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3
{
public:
	float x, y, z;

public:
	Vector3();
	Vector3(float p_x, float p_y, float p_z);
	virtual ~Vector3();

	// Vector operators
	Vector3 operator+( const Vector3& p_other ) const;
	Vector3 operator-( const Vector3& p_other ) const;
	float dot(const Vector3& p_other) const;
	Vector3 cross(const Vector3& p_other) const;

	// Scalar operators
	Vector3 operator*( const float& p_factor ) const;
	Vector3 operator/( const float& p_denominator ) const;

	// Negative
	Vector3 operator-() const;

	void normalize();

	float length() const;
	float lengthSquared() const;

	// Static standard setups.
	static Vector3 vecZero();
	static Vector3 vecMin();
	static Vector3 vecMax();

private:
	static const Vector3 m_vecZero;
	static const Vector3 m_vecMin;
	static const Vector3 m_vecMax;

};

#endif