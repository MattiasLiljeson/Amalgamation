#ifndef VERTEXSTRUCTURES_H
#define VERTEXSTRUCTURES_H

#include "Utility.h"

//Vertex - Position, Color
struct VertexPC
{
	AglVector3 position;
	AglVector4 color;
	VertexPC()
	{
	}
	VertexPC(AglVector3 position, AglVector4 color)
	{
		this->position = position;
		this->color = color;
	}
};
//Vertex - Position, Normal, TexCoord
struct VertexPNT
{
	AglVector3 position;
	AglVector3 normal;
	AglVector2 texCoord;
	VertexPNT()
	{
	}
	VertexPNT(AglVector3 position, AglVector3 normal, AglVector2 texCoord)
	{
		this->position = position;
		this->normal = normal;
		this->texCoord = texCoord;
	}
};

#endif