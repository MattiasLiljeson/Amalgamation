#include "AglMatrix3x3.h"

//Constructors 
AglMatrix3x3::AglMatrix3x3()
{
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
	data[5] = 0;
	data[6] = 0;
	data[7] = 0;
	data[8] = 0;
}
AglMatrix3x3::AglMatrix3x3(float p11, float p12, float p13,
			 float p21, float p22, float p23,
			 float p31, float p32, float p33)
{
	data[0] = p11;
	data[1] = p12;
	data[2] = p13;
	data[3] = p21;
	data[4] = p22;
	data[5] = p23;
	data[6] = p31;
	data[7] = p32;
	data[8] = p33;
}

//Destructor
AglMatrix3x3::~AglMatrix3x3()
{

}

//Operators
float& AglMatrix3x3::operator[](const int& pIndex)
{
	return data[pIndex];
}
const float AglMatrix3x3::operator[](const int& pIndex) const
{
	return data[pIndex];
}

AglMatrix3x3 AglMatrix3x3::operator*(const AglMatrix3x3& pOther) const
{
	AglMatrix3x3 product;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				product[i*3+j] += data[i*3+k] * pOther.data[k*3+j];
			}
		}
	}
	return product;
}
AglMatrix3x3& AglMatrix3x3::operator*=(const AglMatrix3x3& pOther)
{
	AglMatrix3x3 product;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				product[i*3+j] += data[i*3+k] * pOther.data[k*3+j];
			}
		}
	}
	for (int i = 0; i < 9; i++)
		data[i] = product.data[i];
	return (*this);
}