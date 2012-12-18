#include "AglBoxMesh.h"

AglBoxMesh::AglBoxMesh()
{
	positions = vector<AglVector3>(24);
	normals = vector<AglVector3>(24);
	texCoord = vector<AglVector2>(24);

	indices = vector<unsigned int>(36);
	//Front Face
	positions[0] = -AglVector3(0.5f, 0.5f, 0.5f);		normals[0] = AglVector3(0, 0, 1);		texCoord[0] = AglVector2(0, 1);
	positions[1] = -AglVector3(0.5f, -0.5f, 0.5f);		normals[1] =  AglVector3(0, 0, 1);		texCoord[1] = AglVector2(0, 0);
	positions[2] = -AglVector3(-0.5f, -0.5f, 0.5f);		normals[2] =  AglVector3(0, 0, 1); 		texCoord[2] = AglVector2(1, 0);
	positions[3] = -AglVector3(-0.5f, 0.5f, 0.5f);		normals[3] =  AglVector3(0, 0, 1);		texCoord[3] = AglVector2(1, 1);

	//Back Face
	positions[4] = -AglVector3(0.5f, 0.5f, -0.5f);		normals[4] =  AglVector3(0, 0, -1);		texCoord[4] = AglVector2(0, 1);
	positions[5] = -AglVector3(-0.5f, 0.5f, -0.5f);		normals[5] =  AglVector3(0, 0, -1);		texCoord[5] = AglVector2(0, 0);
	positions[6]  =-AglVector3(-0.5f, -0.5f, -0.5f);	normals[6] =  AglVector3(0, 0, -1); 	texCoord[6] = AglVector2(1, 0);
	positions[7] = -AglVector3(0.5f, -0.5f, -0.5f);		normals[7] =  AglVector3(0, 0, -1);		texCoord[7] = AglVector2(1, 1);

	//Left Face
	positions[8] = -AglVector3(0.5f, 0.5f, -0.5f);		normals[8] =  AglVector3(-1, 0, 0);		texCoord[8] = AglVector2(0, 1);
	positions[9] = -AglVector3(0.5f, -0.5f, -0.5f);		normals[9] =  AglVector3(-1, 0, 0);		texCoord[9] = AglVector2(0, 0);
	positions[10] =-AglVector3(0.5f, -0.5f, 0.5f);		normals[10] =  AglVector3(-1, 0, 0);	texCoord[10] = AglVector2(1, 0);
	positions[11] =-AglVector3(0.5f, 0.5f, 0.5f);		normals[11] =  AglVector3(-1, 0, 0);	texCoord[11] = AglVector2(1, 1);

	//Right Face
	positions[12] =-AglVector3(-0.5f, 0.5f, -0.5f);		normals[12] =  AglVector3(1, 0, 0);		texCoord[12] = AglVector2(0, 1);
	positions[13] =-AglVector3(-0.5f, 0.5f, 0.5f);		normals[13] =  AglVector3(1, 0, 0);		texCoord[13] = AglVector2(0, 0);
	positions[14] =-AglVector3(-0.5f, -0.5f, 0.5f);		normals[14] =  AglVector3(1, 0, 0);		texCoord[14] = AglVector2(1, 0);
	positions[15] =-AglVector3(-0.5f, -0.5f, -0.5f);	normals[15] =  AglVector3(1, 0, 0);		texCoord[15] = AglVector2(1, 1);

	//Top Face
	positions[16] =-AglVector3(-0.5f, -0.5f, -0.5f);	normals[16] =  AglVector3(0, 1, 0);		texCoord[16] = AglVector2(0, 1);
	positions[17] =-AglVector3(-0.5f, -0.5f, 0.5f);		normals[17] =  AglVector3(0, 1, 0);		texCoord[17] = AglVector2(0, 0);
	positions[18] =-AglVector3(0.5f, -0.5f, 0.5f);		normals[18] =  AglVector3(0, 1, 0);		texCoord[18] = AglVector2(1, 0);
	positions[19] =-AglVector3(0.5f, -0.5f, -0.5f);		normals[19] =  AglVector3(0, 1, 0);		texCoord[19] = AglVector2(1, 1);

	//Bottom Face
	positions[20] =-AglVector3(-0.5f, 0.5f, -0.5f);		normals[20] =  AglVector3(0, -1, 0);	texCoord[20] = AglVector2(0, 1);
	positions[21] =-AglVector3(0.5f, 0.5f, -0.5f);		normals[21] =  AglVector3(0, -1, 0);	texCoord[21] = AglVector2(0, 0);
	positions[22] =-AglVector3(0.5f, 0.5f, 0.5f);		normals[22] =  AglVector3(0, -1, 0);	texCoord[22] = AglVector2(1, 0);
	positions[23] =-AglVector3(-0.5f, 0.5f, 0.5f);		normals[23] =  AglVector3(0, -1, 0);	texCoord[23] = AglVector2(1, 1);

	int curr = 0;
	for (int i = 0; i < 6; i++)
	{
		int b = i * 4;
		indices[curr++] = b;
		indices[curr++] = b+1;
		indices[curr++] = b+2;

		indices[curr++] = b;
		indices[curr++] = b+2;
		indices[curr++] = b+3;
	}
}
AglBoxMesh::~AglBoxMesh()
{

}