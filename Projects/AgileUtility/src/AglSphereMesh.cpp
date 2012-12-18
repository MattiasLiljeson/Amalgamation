#include "AglSphereMesh.h"

AglSphereMesh::AglSphereMesh()
{
	int pPerimiterPoints = 20;
	float PI = 3.141593f;
	positions = vector<AglVector3>(pPerimiterPoints * (pPerimiterPoints - 1) + 2);
	normals = vector<AglVector3>(pPerimiterPoints * (pPerimiterPoints - 1) + 2);
	texCoord = vector<AglVector2>(pPerimiterPoints * (pPerimiterPoints - 1) + 2);

	float step = (float)(2 * PI / pPerimiterPoints);

	int curr = 0;
	for (int i = 0; i < pPerimiterPoints; i++)
	{
		for (int j = 1; j < pPerimiterPoints; j++)
		{
			float x = (float)cos(step * i);
			float y = 2 * j / (float)pPerimiterPoints - 1.0f;
			float z = (float)sin(step * i);
			float frac = (float)cos(PI * y  / 2);// 1 - y * y;
			AglVector3 pos = AglVector3(x * frac, y, z * frac);
			pos.normalize();

			float texu = (float)(0.5f * asin(pos[0]) / PI) + 0.5f;
			float texv = (float)(0.5f * asin(pos[1]) / PI) + 0.5f;
			texu = 0.5f * pos.x + 0.5f;
			texv = -0.5f * pos.y + 0.5f;

			positions[curr] = pos;
			normals[curr] = pos;
			texCoord[curr] = AglVector2(texu, texv);
			curr++;
		}
	}
	//v[v.size() - 2] = new VertexPositionNormalTexture(new AglVector3(0, -1, 0), new AglVector3(0, -1, 0), new Vector2(0, 0.0f));
	//v[v.size() - 1] = new VertexPositionNormalTexture(new AglVector3(0, 1, 0), new AglVector3(0, 1, 0), new Vector2(0, 0.0f));

	positions[positions.size() - 2] = AglVector3(0, -1, 0);
	normals[positions.size() - 2] = AglVector3(0, -1, 0);
	texCoord[positions.size() - 2] = AglVector2(0, 1);

	positions[positions.size() - 1] = AglVector3(0, 1, 0);
	normals[positions.size() - 1] = AglVector3(0, 1, 0);
	texCoord[positions.size() - 1] = AglVector2(0, 0);

	indices = vector<unsigned int>(pPerimiterPoints * (pPerimiterPoints-2)*6 + pPerimiterPoints*3*2);
	curr = 0;
	for (int i = 0; i < pPerimiterPoints; i++)
	{
		for (int j = 0; j < pPerimiterPoints-2; j++)
		{
			int c1 = i * (pPerimiterPoints - 1) + j; 
			int c2 = (i+1) * (pPerimiterPoints - 1) + j;
			indices[curr++] = c1;
			indices[curr++] = c1 + 1;
			indices[curr++] = c2 % (positions.size()-2);
			indices[curr++] = c1 + 1;
			indices[curr++] = (c2 + 1) % (positions.size()-2);
			indices[curr++] = c2 % (positions.size()-2);
		}
	}
	for (int i = 0; i < pPerimiterPoints; i++)
	{
		int c1 = i * (pPerimiterPoints - 1);
		int c2 = ((i + 1) * (pPerimiterPoints - 1)) % (positions.size() - 2);
		indices[curr++] = positions.size() - 2;
		indices[curr++] = c1;
		indices[curr++] = c2;

		c1 = i * (pPerimiterPoints - 1) + pPerimiterPoints-2;
		c2 = ((i + 1) * (pPerimiterPoints - 1) + pPerimiterPoints-2) % (positions.size() - 2);
		indices[curr++] = positions.size() - 1;
		indices[curr++] = c2;
		indices[curr++] = c1;
	}
}

AglSphereMesh::~AglSphereMesh()
{

}