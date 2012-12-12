#include "PhyUtility.h"

AglVector4 AglVec4MatrixMult(AglVector4& vec, AglMatrix mat)
{
	vec.transform(mat);
	return vec;
	/*vec = AglVector4(mat[0] *vec.x + mat[4] *vec.y + mat[8] * vec.z + mat[12] *vec.w,
		mat[1] *vec.x + mat[5] *vec.y + mat[9] * vec.z + mat[13] *vec.w,
		mat[2] *vec.x + mat[6] *vec.y + mat[10] *vec.z + mat[14] *vec.w,
		mat[3] *vec.x + mat[7] *vec.y + mat[11] *vec.z + mat[15] *vec.w);
	return vec;*/
}
AglVector3	AglVec3Transform(AglVector3& vec, AglMatrix mat)
{
	vec.transform(mat);
	return vec;
	/*AglVector4 v4(vec[0], vec[1], vec[2], 1);
	AglVec4MatrixMult(v4, mat);
	vec = AglVector3(v4.x, v4.y, v4.z);
	return vec;*/
}
AglVector3 AglVec3TransformNormal(AglVector3& vec, AglMatrix mat)
{
	vec.transformNormal(mat);
	return vec;
	/*AglVector4 v4(vec[0], vec[1], vec[2], 0);
	AglVec4MatrixMult(v4, mat);
	vec = AglVector3(v4.x, v4.y, v4.z);
	return vec;*/
}

float AglDet(AglVector3 pV1, AglVector3 pV2, AglVector3 pV3)
{
	float r1 = (pV1[0]*pV2[1]*pV3[2]) + (pV2[0]*pV3[1]*pV1[2]) + (pV3[0]*pV1[1]*pV2[2]);
	float r2 = (pV1[0]*pV3[1]*pV2[2]) + (pV2[0]*pV1[1]*pV3[2]) + (pV3[0]*pV2[1]*pV1[2]);
	return r1 - r2;
}
float AglRandomFloat()
{
	int limit = RAND_MAX;
	float number = (float)(rand() % limit+0)/RAND_MAX;
	return number;
}
float AglRandomFloat(float minValue, float maxValue)
{
	return AglRandomFloat()*(maxValue-minValue)+minValue;
}