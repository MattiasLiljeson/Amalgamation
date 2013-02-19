#include "RandomUtil.h"
#include <stdlib.h>
#include <math.h>

float RandomUtil::randomSingle()
{
	return (float)rand()/(float)RAND_MAX;
}

float RandomUtil::randomInterval( float p_min, float p_max )
{
	return p_min + (float)rand() / (float)RAND_MAX * (p_max-p_min);
}

void RandomUtil::randomEvenlyDistributedSphere( float* out_x, float* out_y, float* out_z )
{
	float u = randomInterval(-1.0f, 1.0f);
	float theta = randomInterval(0.0f, 2.0f * 3.1415926535f);
	*out_x = cosf(theta)*sqrtf(1.0f - u * u);
	*out_y = sinf(theta)*sqrtf(1.0f - u * u);
	*out_z = u;
}