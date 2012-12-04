#include "AglMatrix.h"

//AGLMATRIX

//Constructors 
AglMatrix::AglMatrix()
{
	data[0]		= 0;
	data[1]		= 0;
	data[2]		= 0;
	data[3]		= 0;
	data[4]		= 0;
	data[5]		= 0;
	data[6]		= 0;
	data[7]		= 0;
	data[8]		= 0;
	data[9]		= 0;
	data[10]	= 0;
	data[11]	= 0;
	data[12]	= 0;
	data[13]	= 0;
	data[14]	= 0;
	data[15]	= 0;
}
AglMatrix::AglMatrix(float p11, float p12, float p13, float p14,
						float p21, float p22, float p23, float p24,
							float p31, float p32, float p33, float p34,
								float p41, float p42, float p43, float p44)
{
	data[0]		= p11;
	data[1]		= p12;
	data[2]		= p13;
	data[3]		= p14;
	data[4]		= p21;
	data[5]		= p22;
	data[6]		= p23;
	data[7]		= p24;
	data[8]		= p31;
	data[9]		= p32;
	data[10]	= p33;
	data[11]	= p34;
	data[12]	= p41;
	data[13]	= p42;
	data[14]	= p43;
	data[15]	= p44;
}
AglMatrix::AglMatrix(float* pData)
{
	data[0]		= pData[0];
	data[1]		= pData[1];
	data[2]		= pData[2];
	data[3]		= pData[3];
	data[4]		= pData[4];
	data[5]		= pData[5];
	data[6]		= pData[6];
	data[7]		= pData[7];
	data[8]		= pData[8];
	data[9]		= pData[9];
	data[10]	= pData[10];
	data[11]	= pData[11];
	data[12]	= pData[12];
	data[13]	= pData[13];
	data[14]	= pData[14];
	data[15]	= pData[15];
}

//Destructor
AglMatrix::~AglMatrix()
{

}

	//Operators
float& AglMatrix::operator[](const int& pIndex)
{
	return data[pIndex];
}
const float AglMatrix::operator[](const int& pIndex) const
{
	return data[pIndex];
}
AglMatrix AglMatrix::operator+(const AglMatrix& pOther)
{
	AglMatrix sum;
	for (int i = 0; i < 16; i++)
		sum[i] = data[i] + pOther[i];
	return sum;
}
AglMatrix& AglMatrix::operator+=(const AglMatrix& pOther)
{
	for (int i = 0; i < 16; i++)
		data[i] += pOther[i];
	return (*this);
}
AglMatrix AglMatrix::operator-(const AglMatrix& pOther)
{
	AglMatrix diff;
	for (int i = 0; i < 16; i++)
		diff[i] = data[i] - pOther[i];
	return diff;
}
AglMatrix& AglMatrix::operator-=(const AglMatrix& pOther)
{
	for (int i = 0; i < 16; i++)
		data[i] -= pOther[i];
	return (*this);
}
AglMatrix AglMatrix::operator*(const AglMatrix& pOther)
{
	AglMatrix product;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				product[i*4+j] += data[i*4+k] * pOther.data[k*4+j];
			}
		}
	}
	return product;
}
AglMatrix& AglMatrix::operator*=(const AglMatrix& pOther)
{
	AglMatrix product;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				product[i*4+j] += data[i*4+k] * pOther.data[k*4+j];
			}
		}
	}
	for (int i = 0; i < 16; i++)
		data[i] = product.data[i];
	return (*this);
}

AglMatrix AglMatrix::operator*(const float& pFactor)
{
	AglMatrix product;
	for (int i = 0; i < 16; i++)
		product.data[i] = pFactor * data[i];
	return product;
}
AglMatrix& AglMatrix::operator*=(const float& pFactor)
{
	for (int i = 0; i < 16; i++)
		data[i] *= pFactor;
	return (*this);
}

//Static Functions

//Returns an identity 4x4matrix
AglMatrix AglMatrix::identityMatrix()
{
	return AglMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

//Returns a 4x4matrix filled with zeroes
AglMatrix AglMatrix::zeroMatrix()
{
	return AglMatrix();
}

//Returns the determinant of a soucre 4x4matrix
float AglMatrix::determinant(AglMatrix pSource)
{
	AglMatrix& m = pSource;
	float det = m[0]*m[5]*m[10]*m[15] + m[0]*m[6]*m[11]*m[13] + m[0]*m[7]*m[9]*m[14];
	det +=		m[1]*m[4]*m[11]*m[14] + m[1]*m[6]*m[8]*m[15] + m[1]*m[7]*m[10]*m[12];
	det +=		m[2]*m[4]*m[9]*m[15] + m[2]*m[5]*m[11]*m[12] + m[2]*m[7]*m[8]*m[13];
	det +=		m[3]*m[4]*m[10]*m[13] + m[3]*m[5]*m[8]*m[14] + m[3]*m[6]*m[9]*m[12];
	det -=		(m[0]*m[5]*m[11]*m[14] + m[0]*m[6]*m[9]*m[15] + m[0]*m[7]*m[10]*m[13]);
	det -=		(m[1]*m[4]*m[10]*m[15] + m[1]*m[6]*m[11]*m[12] + m[1]*m[7]*m[8]*m[14]);
	det -=		(m[2]*m[4]*m[11]*m[13] + m[2]*m[5]*m[8]*m[15] + m[2]*m[7]*m[9]*m[12]);
	det -=		(m[3]*m[4]*m[9]*m[14] + m[3]*m[5]*m[10]*m[12] + m[3]*m[6]*m[8]*m[13]);
	return det;
}

//Returns the transpose of a source 4x4matrix
AglMatrix AglMatrix::transpose(AglMatrix pSource)
{
	AglMatrix transpose;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			transpose[i * 4 + j] = pSource[j * 4 + i];
	return transpose;
}

//Returns the inverse of a source 4x4matrix
AglMatrix AglMatrix::inverse(AglMatrix pSource)
{
	// Inspiration from: http://download.intel.com/design/PentiumIII/sml/24504301.pdf

	//Destination
	AglMatrix dst;

	//Contains temporary cofactors
	float temp[12];

	//Transposed source matrix
	AglMatrix src = transpose(pSource);

	//Cofactors for the first 8 elements in the array - optimized based
	//on cramers rule multiplications
	temp[0]		= src[10]	* src[15];
	temp[1]		= src[11]	* src[14];
	temp[2]		= src[9]	* src[15];
	temp[3]		= src[11]	* src[13];
	temp[4]		= src[9]	* src[14];
	temp[5]		= src[10]	* src[13];
	temp[6]		= src[8]	* src[15];
	temp[7]		= src[11]	* src[12];
	temp[8]		= src[8]	* src[14];
	temp[9]		= src[10]	* src[12];
	temp[10]	= src[8]	* src[13];
	temp[11]	= src[9]	* src[12];

	//Compute values for the first 8 matrix elements
	dst[0] =	temp[0]*src[5] + temp[3]*src[6] + temp[4] *src[7];
	dst[0] -=	temp[1]*src[5] + temp[2]*src[6] + temp[5] *src[7];
	dst[1] =	temp[1]*src[4] + temp[6]*src[6] + temp[9] *src[7];
	dst[1] -=	temp[0]*src[4] + temp[7]*src[6] + temp[8] *src[7];
	dst[2] =	temp[2]*src[4] + temp[7]*src[5] + temp[10]*src[7];
	dst[2] -=	temp[3]*src[4] + temp[6]*src[5] + temp[11]*src[7];
	dst[3] =	temp[5]*src[4] + temp[8]*src[5] + temp[11]*src[6];
	dst[3] -=	temp[4]*src[4] + temp[9]*src[5] + temp[10]*src[6];
	dst[4] =	temp[1]*src[1] + temp[2]*src[2] + temp[5] *src[3];
	dst[4] -=	temp[0]*src[1] + temp[3]*src[2] + temp[4] *src[3];
	dst[5] =	temp[0]*src[0] + temp[7]*src[2] + temp[8] *src[3];
	dst[5] -=	temp[1]*src[0] + temp[6]*src[2] + temp[9] *src[3];
	dst[6] =	temp[3]*src[0] + temp[6]*src[1] + temp[11]*src[3];
	dst[6] -=	temp[2]*src[0] + temp[7]*src[1] + temp[10]*src[3];
	dst[7] =	temp[4]*src[0] + temp[9]*src[1] + temp[10]*src[2];
	dst[7] -=	temp[5]*src[0] + temp[8]*src[1] + temp[11]*src[2];

	//Cofactors for the second 8 elements in the array - optimized based
	//on cramers rule multiplications
	temp[0]		= src[2]	*	src[7];
	temp[1]		= src[3]	*	src[6];
	temp[2]		= src[1]	*	src[7];
	temp[3]		= src[3]	*	src[5];
	temp[4]		= src[1]	*	src[6];
	temp[5]		= src[2]	*	src[5];
	temp[6]		= src[0]	*	src[7];
	temp[7]		= src[3]	*	src[4];
	temp[8]		= src[0]	*	src[6];
	temp[9]		= src[2]	*	src[4];
	temp[10]	= src[0]	*	src[5];
	temp[11]	= src[1]	*	src[4];

	//Compute values for the second 8 matrix elements
	dst[8] =	temp[0]	 * src[13] + temp[3]  * src[14] + temp[4]  * src[15];
	dst[8] -=	temp[1]	 * src[13] + temp[2]  * src[14] + temp[5]  * src[15];
	dst[9] =	temp[1]	 * src[12] + temp[6]  * src[14] + temp[9]  * src[15];
	dst[9] -=	temp[0]	 * src[12] + temp[7]  * src[14] + temp[8]  * src[15];
	dst[10] =	temp[2]	 * src[12] + temp[7]  * src[13] + temp[10] * src[15];
	dst[10] -=	temp[3]	 * src[12] + temp[6]  * src[13] + temp[11] * src[15];
	dst[11] =	temp[5]	 * src[12] + temp[8]  * src[13] + temp[11] * src[14];
	dst[11] -=	temp[4]	 * src[12] + temp[9]  * src[13] + temp[10] * src[14];
	dst[12] =	temp[2]	 * src[10] + temp[5]  * src[11] + temp[1]  * src[9];
	dst[12] -=	temp[4]	 * src[11] + temp[0]  * src[9]  + temp[3]  * src[10];
	dst[13] =	temp[8]	 * src[11] + temp[0]  * src[8]  + temp[7]  * src[10];
	dst[13] -=	temp[6]	 * src[10] + temp[9]  * src[11] + temp[1]  * src[8];
	dst[14] =	temp[6]	 * src[9]  + temp[11] * src[11] + temp[3]  * src[8];
	dst[14] -=	temp[10] * src[11] + temp[2]  * src[8]  + temp[7]  * src[9];
	dst[15] =	temp[10] * src[10] + temp[4]  * src[8]  + temp[9]  * src[9];
	dst[15] -=	temp[8]	 * src[9]  + temp[11] * src[10] + temp[5]  * src[8];

	//Calculate determinant
	float det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];
	det = 1.0f / det;
		
	//Scale the inverted matrix
	for (int i = 0; i < 16; i++)
		dst[i] *= det;
		
	return dst;
}
void AglMatrix::matrixToComponents(AglMatrix pMatrix, AglVector3& pScale, AglQuaternion& pQuaternion, AglVector3& pTranslation)
{
	// http://software.intel.com/sites/default/files/m/d/4/1/d/8/293748.pdf
	// Real-Time Rendering

	//Scale
	AglVector3 row0(pMatrix[0], pMatrix[1], pMatrix[2]);
	AglVector3 row1(pMatrix[4], pMatrix[5], pMatrix[6]);
	AglVector3 row2(pMatrix[8], pMatrix[9], pMatrix[10]);

	float lrow0 = AglVector3::length(row0);
	float lrow1 = AglVector3::length(row1);
	float lrow2 = AglVector3::length(row2);

	pScale[0] = lrow0;
	pScale[1] = lrow1;
	pScale[2] = lrow2;

	row0 *= (1/lrow0);
	row1 *= (1/lrow1);
	row2 *= (1/lrow2);

	//Translation
	pTranslation[0] = pMatrix[12];
	pTranslation[1] = pMatrix[13];
	pTranslation[2] = pMatrix[14];

	//Rotation

	//Find the largest factor.

	float qx, qy, qz, qw;
	if (row0[0] + row1[1] + row2[2] > 0.0f) //Use w
	{
		float t = row0[0] + row1[1] + row2[2] + pMatrix[15];
		float s = 0.5f / sqrt(t);
		
		qw = s * t;
		qz = (row0[1] - row1[0]) * s;
		qy = (row2[0] - row0[2]) * s;
		qx = (row1[2] - row2[1]) * s;
	}
	else if (row0[0] > row1[1] && row0[0] > row2[2]) //Use x
	{
		float t = row0[0] - row1[1] - row2[2] + pMatrix[15];
		float s = 0.5f / sqrt(t);
		
		qx = s * t;
		qy = (row0[1] + row1[0]) * s;
		qz = (row2[0] + row0[2]) * s;
		qw = (row1[2] - row2[1]) * s;
	}
	else if (row1[1] > row2[2]) //Use y
	{
		float t = -row0[0] + row1[1] - row2[2] + pMatrix[15];
		float s = 0.5f / sqrt(t);
		
		qy = s * t;
		qx = (row0[1] + row1[0]) * s;
		qw = (row2[0] - row0[2]) * s;
		qz = (row1[2] + row2[1]) * s;
	}
	else //Use z
	{
		float t = -row0[0] - row1[1] + row2[2] + pMatrix[15];
		float s = 0.5f / sqrt(t);
		
		qz = s * t;
		qw = (row0[1] - row1[0]) * s;
		qx = (row2[0] + row0[2]) * s;
		qy = (row1[2] + row2[1]) * s;
	}
	pQuaternion = AglQuaternion(qx, qy, qz, qw);
}
void AglMatrix::componentsToMatrix(AglMatrix& pMatrix, const AglVector3& pScale, const AglQuaternion& pQuaternion, const AglVector3& pTranslation)
{
	//Scale
	AglMatrix S(pScale[0], 0, 0, 0, 0, pScale[1], 0, 0, 0, 0, pScale[2], 0, 0, 0, 0, 1);
	//Translation
	AglMatrix T(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, pTranslation[0], pTranslation[1], pTranslation[2], 1);
	
	//Rotation
	//Based on one simple understanding. Given a quaternion rotation transform
	//of qpq* two separate matrix-vector transforms can be derived.
	//The following represents the concatenation of the two matrices.
	float qx = pQuaternion.u[0];
	float qy = pQuaternion.u[1];
	float qz = pQuaternion.u[2];
	float qw = pQuaternion.v;
	AglMatrix R;
	R[0] = 1 - 2*(qy*qy + qz*qz);
	R[1] = 2*(qx*qy + qw*qz);
	R[2] = 2*(qx*qz - qw*qy);
	R[3] = 0;

	R[4] = 2*(qx*qy - qw*qz);
	R[5] = 1 - 2*(qx*qx + qz*qz);
	R[6] = 2*(qy*qz + qw*qx);
	R[7] = 0;

	R[8] = 2*(qx*qz + qw*qy);
	R[9] = 2*(qy*qz - qw*qx);
	R[10] = 1 - 2*(qx*qx + qy*qy);
	R[11] = 0;

	R[12] = 0;
	R[13] = 0;
	R[14] = 0;
	R[15] = 1;

	pMatrix = S * R * T;
}