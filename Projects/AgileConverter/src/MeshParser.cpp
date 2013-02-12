#include "MeshParser.h"
#include "FBXParser.h"

float dot(AglVector3 a1, AglVector3 a2)
{
	return a1[0] * a2[0] + a1[1] * a2[1] + a1[2] * a2[2];
}

MeshParser::MeshParser(FBXParser* pParent, FbxMesh* pMesh, FbxNode* pNode)
{
	mParent = pParent;
	mMesh = pMesh;
	mNode = pNode;
}
void MeshParser::Parse()
{
	if (!mMesh->IsTriangleMesh())
	{
		FbxGeometryConverter conv(FBXManager::GetInstance()->GetManager());
		mMesh = conv.TriangulateMesh(mMesh);
	}
	int polycount = mMesh->GetPolygonCount();

	vector<AglVector3> positions	= GetPositions();
	vector<AglVector3> normals		= GetNormals();
	//vector<AglVector3> tangents		= GetTangents();
	//vector<AglVector3> bitangents	= GetBitangents();
	vector<AglVector2> UVs			= GetUV();
	if (UVs.size() == 0)
		UVs = vector<AglVector2>(normals.size());

	//Indices
	int count = mMesh->GetPolygonVertexCount();
	int* controlIndices = mMesh->GetPolygonVertices();
	unsigned int* indices = new unsigned int[count];
	for (int i = 0; i < count; i++)
	{
		indices[i] = i;
	}

	bool nMapMode = mMesh->GetLayer(0)->GetNormals()->GetMappingMode() == FbxLayerElement::eByPolygonVertex;
	bool uvMapMode = nMapMode;
	if (mMesh->GetLayerCount(FbxLayerElement::eUV) > 0)
		uvMapMode = mMesh->GetLayer(0)->GetUVs()->GetMappingMode() == FbxLayerElement::eByPolygonVertex;

	//USE A HASH FUNCTION TO CREATE THE VERTEX ARRAY. THIS REMOVES ANY DUPLICATED
	//VERTICES WITH MINIMUM PROCESSING OVERHEAD.
	vector<vector<HashData>> hash(mMesh->GetControlPointsCount());
	vector<AglVertexSTBN> verts;
	vector<int>			  controlIndex;
	for (int i = 0; i < count; i++)
	{
		int p, n, t;
		p = controlIndices[i];
		if (nMapMode)
			n = i;
		else
			n = controlIndices[i];
		if (uvMapMode)
			t = i;
		else
			t = controlIndices[i];

		int index = -1;
		for (unsigned int j = 0; j < hash[p].size(); j++)
		{
			if (hash[p][j].n == n && hash[p][j].t == t)
			{
				index = hash[p][j].index;
				break;
			}
		}
		if (index == -1)
		{
			index = verts.size();

			AglVertexSTBN v;
			v.position = positions[p];
			v.normal = normals[n];
			v.texCoord = UVs[t];
			verts.push_back(v);
			controlIndex.push_back(p);

			HashData newHD;
			newHD.index = index;
			newHD.n = n;
			newHD.t = t;
			hash[p].push_back(newHD);
		}
		indices[i] = index;
	}

	ComputeTangentSpace(verts, indices, count);
	AglVertexSTBN* vertices = new AglVertexSTBN[verts.size()];
	int* indexToControl = new int[controlIndex.size()];
	for (unsigned int i = 0; i < verts.size(); i++)
		vertices[i] = verts[i];
	for (unsigned int i = 0; i < controlIndex.size(); i++)
		indexToControl[i] = controlIndex[i];

	MeshData* m = new MeshData();
	m->ID = -1;
	m->VertexFormat = AGL_VERTEX_FORMAT_PNTTBN;
	m->Indices = indices;
	m->IndicesCount = count;
	m->Name = mNode->GetName();
	m->Vertices = vertices;
	m->VertexCount = verts.size();
	m->ControlIndices = indexToControl;
	m->Source = mMesh;
	m->SourceNode = mNode;

	FbxMatrix transform = m->SourceNode->EvaluateGlobalTransform();
	for (int row = 0; row < 4; row++)
		for (int column = 0; column < 4; column++)
			m->Transform[row*4 + column] = (float)transform.Get(row, column);

	mParent->AddMesh(m);

	FbxNode* parent = m->SourceNode->GetParent();
	while (parent)
	{
		const char* name = parent->GetName();
		parent = parent->GetParent();
	}

	//CREATE THE MESH WITHOUT USING A HASH TABLE TO REMOVE DUPLICATED VERTICES. COSTS MORE MEMORY
	/*AglVector3 p, n;
	AglVector2 uv;

	AglVertexSPNT* vertices = new AglVertexSPNT[count];
	for (int i = 0; i < count; i++)
	{
		vertices[i].Position = positions[controlIndices[i]];
		if (nMapMode)
			vertices[i].Normal = normals[i];
		else
			vertices[i].Normal = normals[controlIndices[i]];
		if (uvMapMode)
			vertices[i].TexCoord = UVs[i];
		else
			vertices[i].TexCoord = UVs[controlIndices[i]];

		//vertices[i] = AglVertexSPNT(p, n, uv);
	}

	MeshData* m = new MeshData();
	m->ID = -1;
	strcpy_s(m->VertexFormat, "SPNT");
	m->Indices = indices;
	m->IndicesCount = count;
	m->Name = mMesh->GetName();
	m->Vertices = vertices;
	m->VertexCount = count;
	m->ControlIndices = controlIndices;
	m->Source = mMesh;

	mParent->AddMesh(m);*/
}
vector<AglVector3> MeshParser::GetPositions()
{
	FbxVector4* points = mMesh->GetControlPoints();
	int pointcount = mMesh->GetControlPointsCount();
	vector<AglVector3> positions;
	for (int i = 0; i < pointcount; i++)
	{
		FbxVector4 point = points[i];
		float x = (float)point.mData[0];
		float y = (float)point.mData[1];
		float z = (float)point.mData[2];
		float w = (float)point.mData[3];
		positions.push_back(AglVector3(x, y, z));
	}
	return positions;
}
vector<AglVector3> MeshParser::GetNormals()
{
	vector<AglVector3> normalvec;
	//The number of normal layers should always be 1. 
	int normalLayers = mMesh->GetLayerCount(FbxLayerElement::eNormal);
	FbxLayerElementNormal* normals = mMesh->GetLayer(0)->GetNormals();
	FbxLayerElement::EMappingMode mappingmode =  normals->GetMappingMode();
	FbxLayerElement::EReferenceMode refmode = normals->GetReferenceMode();
	if (mappingmode == FbxLayerElement::eByControlPoint)
	{
		//One normal per control point (Position Vertex)
		for (int i = 0; i < mMesh->GetControlPointsCount(); i++)
		{
			int normIndex = 0;
			if (refmode == FbxLayerElement::eDirect)
				normIndex = i;
			else if (refmode == FbxLayerElement::eIndexToDirect)
				normIndex = normIndex = normals->GetIndexArray().GetAt(i);

			FbxVector4 normal = normals->GetDirectArray().GetAt(normIndex);
			float x = (float)normal.mData[0];
			float y = (float)normal.mData[1];
			float z = (float)normal.mData[2];
			normalvec.push_back(AglVector3(x, y, z));
		}
	}
	else if (mappingmode == FbxLayerElement::eByPolygonVertex)
	{
		//One normal per PolygonVertex (A Position Vertex belonging to a certain polygon)
		//For a triangulated mesh there would be 3 * trianglecount number of normals.
		int polycount = mMesh->GetPolygonCount();
		for (int i = 0; i < polycount*3; i++)
		{
			int normIndex = 0;
			if (refmode == FbxLayerElement::eDirect)
				normIndex = i;
			else if (refmode == FbxLayerElement::eIndexToDirect)
				normIndex = normIndex = normals->GetIndexArray().GetAt(i);

			FbxVector4 normal = normals->GetDirectArray().GetAt(normIndex);
			float x = (float)normal.mData[0];
			float y = (float)normal.mData[1];
			float z = (float)normal.mData[2];
			normalvec.push_back(AglVector3(x, y, z));
		}
	}
	return normalvec;
}
vector<AglVector3> MeshParser::GetTangents()
{
	vector<AglVector3> tangentvec;
	//The number of normal layers should always be 1. 
	int tangentLayers = mMesh->GetLayerCount(FbxLayerElement::eTangent);
	FbxLayerElementTangent* tangents = mMesh->GetLayer(0)->GetTangents();
	FbxLayerElement::EMappingMode mappingmode =  tangents->GetMappingMode();
	FbxLayerElement::EReferenceMode refmode = tangents->GetReferenceMode();
	if (mappingmode == FbxLayerElement::eByControlPoint)
	{
		//One tangent per control point (Position Vertex)
		for (int i = 0; i < mMesh->GetControlPointsCount(); i++)
		{
			int tanIndex = 0;
			if (refmode == FbxLayerElement::eDirect)
				tanIndex = i;
			else if (refmode == FbxLayerElement::eIndexToDirect)
				tanIndex = tanIndex = tangents->GetIndexArray().GetAt(i);

			FbxVector4 tangent = tangents->GetDirectArray().GetAt(tanIndex);
			float x = (float)tangent.mData[0];
			float y = (float)tangent.mData[1];
			float z = (float)tangent.mData[2];
			tangentvec.push_back(AglVector3(x, y, z));
		}
	}
	else if (mappingmode == FbxLayerElement::eByPolygonVertex)
	{
		//One normal per PolygonVertex (A Position Vertex belonging to a certain polygon)
		//For a triangulated mesh there would be 3 * trianglecount number of normals.
		int polycount = mMesh->GetPolygonCount();
		for (int i = 0; i < polycount*3; i++)
		{
			int tanIndex = 0;
			if (refmode == FbxLayerElement::eDirect)
				tanIndex = i;
			else if (refmode == FbxLayerElement::eIndexToDirect)
				tanIndex = tanIndex = tangents->GetIndexArray().GetAt(i);

			FbxVector4 tangent = tangents->GetDirectArray().GetAt(tanIndex);
			float x = (float)tangent.mData[0];
			float y = (float)tangent.mData[1];
			float z = (float)tangent.mData[2];
			tangentvec.push_back(AglVector3(x, y, z));
		}
	}
	return tangentvec;
}
vector<AglVector3> MeshParser::GetBitangents()
{
	vector<AglVector3> bitangentvec;
	//The number of normal layers should always be 1. 
	int bitangentLayers = mMesh->GetLayerCount(FbxLayerElement::eBiNormal);
	FbxLayerElementBinormal* bitangents = mMesh->GetLayer(0)->GetBinormals();
	FbxLayerElement::EMappingMode mappingmode =  bitangents->GetMappingMode();
	FbxLayerElement::EReferenceMode refmode = bitangents->GetReferenceMode();
	if (mappingmode == FbxLayerElement::eByControlPoint)
	{
		//One normal per control point (Position Vertex)
		for (int i = 0; i < mMesh->GetControlPointsCount(); i++)
		{
			int bitanIndex = 0;
			if (refmode == FbxLayerElement::eDirect)
				bitanIndex = i;
			else if (refmode == FbxLayerElement::eIndexToDirect)
				bitanIndex = bitanIndex = bitangents->GetIndexArray().GetAt(i);

			FbxVector4 bitangent = bitangents->GetDirectArray().GetAt(bitanIndex);
			float x = (float)bitangent.mData[0];
			float y = (float)bitangent.mData[1];
			float z = (float)bitangent.mData[2];
			bitangentvec.push_back(AglVector3(x, y, z));
		}
	}
	else if (mappingmode == FbxLayerElement::eByPolygonVertex)
	{
		//One normal per PolygonVertex (A Position Vertex belonging to a certain polygon)
		//For a triangulated mesh there would be 3 * trianglecount number of normals.
		int polycount = mMesh->GetPolygonCount();
		for (int i = 0; i < polycount*3; i++)
		{
			int bitanIndex = 0;
			if (refmode == FbxLayerElement::eDirect)
				bitanIndex = i;
			else if (refmode == FbxLayerElement::eIndexToDirect)
				bitanIndex = bitanIndex = bitangents->GetIndexArray().GetAt(i);

			FbxVector4 bitan = bitangents->GetDirectArray().GetAt(bitanIndex);
			float x = (float)bitan.mData[0];
			float y = (float)bitan.mData[1];
			float z = (float)bitan.mData[2];
			bitangentvec.push_back(AglVector3(x, y, z));
		}
	}
	return bitangentvec;
}
vector<AglVector2> MeshParser::GetUV()
{
	vector<AglVector2> uvvec;
	//The number of uv layers should always be 1. 
	int uvlayers = mMesh->GetLayerCount(FbxLayerElement::eUV);
	if (uvlayers == 0)
		return uvvec;
	FbxLayerElementUV* UVs = mMesh->GetLayer(0)->GetUVs();
	FbxLayerElement::EMappingMode mappingmode =  UVs->GetMappingMode();
	FbxLayerElement::EReferenceMode refmode = UVs->GetReferenceMode();

	if (mappingmode == FbxLayerElement::eByControlPoint)
	{
		//One UV per control point (Position Vertex)
		for (int i = 0; i < mMesh->GetControlPointsCount(); i++)
		{
			int UVIndex = 0;
			if (refmode == FbxLayerElement::eDirect)
				UVIndex = i;
			else if (refmode == FbxLayerElement::eIndexToDirect)
				UVIndex = UVs->GetIndexArray().GetAt(i);

			FbxVector4 UV = UVs->GetDirectArray().GetAt(UVIndex);
			float u = (float)UV.mData[0];
			float v = (float)(1.0f-UV.mData[1]);
			uvvec.push_back(AglVector2(u, v));
		}
	}
	else if (mappingmode == FbxLayerElement::eByPolygonVertex)
	{
		//One UV per PolygonVertex (A Position Vertex belonging to a certain polygon)
		//For a triangulated mesh there would be 3 * trianglecount number of UVs.
		int polycount = mMesh->GetPolygonCount();
		for (int i = 0; i < polycount*3; i++)
		{
			int UVIndex = 0;
			if (refmode == FbxLayerElement::eDirect)
				UVIndex = i;
			else if (refmode == FbxLayerElement::eIndexToDirect)
				UVIndex = UVs->GetIndexArray().GetAt(i);

			FbxVector4 UV = UVs->GetDirectArray().GetAt(UVIndex);
			float u = (float)UV.mData[0];
			float v = (float)(1.0f-UV.mData[1]);
			uvvec.push_back(AglVector2(u, v));
		}
	}
	return uvvec;
}

void MeshParser::ComputeTangentSpace(vector<AglVertexSTBN>& pVertices, unsigned int* pIndices, int pIndexCount)
{
	vector<AglVector3> tan(pVertices.size());
	vector<AglVector3> bin(pVertices.size());
	for (int i = 0; i < pIndexCount; i+=3)
	{
		AglVector3 p1 = pVertices[pIndices[i]].position;
		AglVector3 p2 = pVertices[pIndices[i+1]].position;
		AglVector3 p3 = pVertices[pIndices[i+2]].position;
		
		AglVector3 q1 = AglVector3(p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]);
		AglVector3 q2 = AglVector3(p3[0] - p1[0], p3[1] - p1[1], p3[2] - p1[2]);

		AglVector2 s = AglVector2(pVertices[pIndices[i + 1]].texCoord[0] - pVertices[pIndices[i]].texCoord[0], pVertices[pIndices[i + 2]].texCoord[0] - pVertices[pIndices[i]].texCoord[0]);
		AglVector2 t = AglVector2(pVertices[pIndices[i + 1]].texCoord[1] - pVertices[pIndices[i]].texCoord[1], pVertices[pIndices[i + 2]].texCoord[1] - pVertices[pIndices[i]].texCoord[1]);

		float frac = 1 / (s[0] * t[1] - s[1] * t[0]);
		float tx = frac * (q1[0] * t[1] - q2[0] * t[0]);
		float ty = frac * (q1[1] * t[1] - q2[1] * t[0]);
		float tz = frac * (q1[2] * t[1] - q2[2] * t[0]);
		float bx = frac * (q2[0] * s[0] - q1[0] * s[1]);
		float by = frac * (q2[1] * s[0] - q1[1] * s[1]);
		float bz = frac * (q2[2] * s[0] - q1[2] * s[1]);
		AglVector3 T(tx, ty, tz);
		AglVector3 B(bx, by, bz);
		tan[pIndices[i]] = AglVector3(tan[pIndices[i]][0] + tx, tan[pIndices[i]][1] + ty, tan[pIndices[i]][2] + tz);
		tan[pIndices[i+1]] = AglVector3(tan[pIndices[i+1]][0] + tx, tan[pIndices[i+1]][1] + ty, tan[pIndices[i+1]][2] + tz);
		tan[pIndices[i+2]] = AglVector3(tan[pIndices[i+2]][0] + tx, tan[pIndices[i+2]][1] + ty, tan[pIndices[i+2]][2] + tz);
		bin[pIndices[i]] = AglVector3(bin[pIndices[i]][0] + bx, bin[pIndices[i]][1] + by, bin[pIndices[i]][2] + bz);
		bin[pIndices[i+1]] = AglVector3(bin[pIndices[i+1]][0] + bx, bin[pIndices[i+1]][1] + by, bin[pIndices[i+1]][2] + bz);
		bin[pIndices[i+2]] = AglVector3(bin[pIndices[i+2]][0] + bx, bin[pIndices[i+2]][1] + by, bin[pIndices[i+2]][2] + bz);
	}
	for (int i = 0; i < pVertices.size(); i++)
	{
		//Gram-Schmidt
		AglVector3 oldTan = tan[i];
		AglVector3 n = pVertices[i].normal;
		float frac = dot(oldTan, n);
		tan[i] = AglVector3(oldTan[0] - frac * n[0], oldTan[1] - frac * n[1], oldTan[2] - frac * n[2]);
		
		float frac1 = dot(n, bin[i]);
		float frac2 = dot(tan[i], bin[i]);
		float frac3 = dot(oldTan, oldTan);
		bin[i] = AglVector3(bin[i][0] - frac1 * n[0] - frac2 * tan[i][0] / frac3,
							bin[i][1] - frac1 * n[1] - frac2 * tan[i][1] / frac3,
							bin[i][2] - frac1 * n[2] - frac2 * tan[i][2] / frac3);



		float l = sqrt(tan[i][0] * tan[i][0] + tan[i][1] * tan[i][1] + tan[i][2] * tan[i][2]);
		tan[i] = AglVector3(tan[i][0] / l, tan[i][1] / l, tan[i][2] / l);
		l = sqrt(bin[i][0] * bin[i][0] + bin[i][1] * bin[i][1] + bin[i][2] * bin[i][2]);
		bin[i] = AglVector3(bin[i][0] / l, bin[i][1] / l, bin[i][2] / l);
		pVertices[i].tangent = tan[i];
		pVertices[i].binormal = bin[i];
		float dot = tan[i][0] * bin[i][0] + tan[i][1] * bin[i][1] + tan[i][2] * bin[i][2];
		dot++;
	}
}