#include "Eigen.h"

void FindSymmetricEigenVectors(float** A, AglVector3& p_eigen1, AglVector3& p_eigen2, AglVector3& p_eigen3)
{
	int n = 3;
	int i, j, k;
	bool done;
	float discr, aii, aij, ajj, aik, ajk, v1, v2, norm;
	float a, b, c;
	AglMatrix eigen = AglMatrix::identityMatrix();
	do
	{
		done = true;
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < i; j++)
			{
				//Calculate eigenvalues for sub 2D-matrix
				aii = b = A[i][i];
				aij = c = A[i][j];
				ajj = a = A[j][j];

				//Do not perform the calculation if the "non-diagonal" element is near zero
				if (abs(c) > 0.000001f)
				{
					discr = AglVector2::length(AglVector2(b-a, 2 * c));
					float p = -(a + b);
					float q = a * b - c * c;
					float root = (float)sqrt((-p / 2) * (-p / 2) - q);
					if ((-p / 2) + root > (-p / 2) - root)
					{
						A[i][i] = (-p / 2) + root;
						A[j][j] = (-p / 2) - root;
					}
					else
					{
						A[j][j] = (-p / 2) + root;
						A[i][i] = (-p / 2) - root;
					}
					A[i][j] = A[j][i] = 0;

					if (aii > ajj)
					{
						v1 = (aii - ajj + discr) / 2;
						v2 = -aij;
					}
					else
					{
						v1 = aij;
						v2 = -(ajj - aii + discr) / 2;
					}
					AglMatrix manip = AglMatrix::identityMatrix();
					norm = AglVector2::length(AglVector2(v1, v2));
					v1 /= norm;
					v2 /= norm;

					manip[j*4+j] = v1;
					manip[i*4+j] = v2;
					manip[j*4+i] = -v2;
					manip[i*4+i] = v1;

					eigen *= manip;

					/* Apply change of basis to the rest of the matrix. */
					for (k = 0; k < n; k++)
					{
						if (k != i && k != j)
						{
							aik = A[i][k];
							ajk = A[j][k];
							A[i][k] = A[k][i] = v1 * aik - v2 * ajk;
							A[j][k] = A[k][j] = v2 * aik + v1 * ajk;
						}
					}
					done = false;
				}
			}
		}
	} while (!done);

	float val1 = A[0][0];
	float val2 = A[1][1];
	float val3 = A[2][2];

	p_eigen1 = AglVector3(eigen[0], eigen[4], eigen[8]);
	p_eigen2 = AglVector3(eigen[1], eigen[5], eigen[9]);
	p_eigen3 = AglVector3(eigen[2], eigen[6], eigen[10]);

	p_eigen1.normalize();
	p_eigen2.normalize();
	p_eigen3.normalize();
}