#ifndef AGLMATRIX3X3_H
#define AGLMATRIX3X3_H

struct AglMatrix3x3
{
	float data[9];

	//Constructors 
	AglMatrix3x3();
	AglMatrix3x3(float p11, float p12, float p13,
		float p21, float p22, float p23,
		float p31, float p32, float p33);

	//Destructor
	~AglMatrix3x3();

	//Operators
	float& operator[](const int& pIndex);
	const float operator[](const int& pIndex) const;

	AglMatrix3x3 operator*(const AglMatrix3x3& pOther) const;
	AglMatrix3x3& operator*=(const AglMatrix3x3& pOther);
};


#endif