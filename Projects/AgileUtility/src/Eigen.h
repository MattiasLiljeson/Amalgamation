#ifndef EIGEN_H
#define EIGEN_H

#include "AglVector3.h"
#include "AglVector2.h"
#include "AglMatrix.h"

void FindSymmetricEigenVectors(float** A, AglVector3& p_eigen1, AglVector3& p_eigen2, AglVector3& p_eigen3);

#endif