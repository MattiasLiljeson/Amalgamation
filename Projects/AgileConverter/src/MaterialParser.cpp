#include "MaterialParser.h"
#include "FBXParser.h"

MaterialParser::MaterialParser(FBXParser* pParent)
{
	mParent = pParent;
}

void MaterialParser::Parse(FbxSurfaceMaterial* pMaterial)
{
	if (!mParent->GetMaterial(pMaterial))
	{
		if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			FbxSurfacePhong* surface = (FbxSurfacePhong*)pMaterial;

			MaterialData* md = new MaterialData();
			md->Source = pMaterial;

			//Add Ambient Color
			FbxDouble3 ambient = surface->Ambient;
			md->Ambient[0] = ambient.mData[0]; md->Ambient[1] = ambient.mData[1]; md->Ambient[2] = ambient.mData[2];

			//Add Diffuse Color
			FbxDouble3 diffuse = surface->Diffuse;
			md->Diffuse[0] = diffuse.mData[0]; md->Diffuse[1] = diffuse.mData[1]; md->Diffuse[2] = diffuse.mData[2];

			//Add Specular Color
			FbxDouble3 specular = surface->Specular;
			md->Specular[0] = specular.mData[0]; md->Specular[1] = specular.mData[1]; md->Specular[2] = specular.mData[2];

            //Add Emissive Color
			FbxDouble3 emissive = surface->Emissive;
			md->Emissive[0] = emissive.mData[0]; md->Emissive[1] = emissive.mData[1]; md->Emissive[2] = emissive.mData[2];

            //Add Opacity
            FbxDouble opacity = 1.0f - surface->TransparencyFactor;
			md->Opacity = opacity;

            //Add Shininess
			FbxDouble shininess = surface->Shininess;
			md->Shininess = shininess;

            //Add Reflectivity
			FbxDouble reflectivity = surface->ReflectionFactor;
			md->Reflectivity = reflectivity;



			//Diffuse Texture
			for (int i = 0; i < surface->Diffuse.GetSrcObjectCount(FbxFileTexture::ClassId); i++)
			{
				FbxFileTexture* t = (FbxFileTexture*)surface->Diffuse.GetSrcObject(FbxTexture::ClassId, i);
				string path = ParseTexture(t);
				for (int j = 0; j < path.length(); j++)
				{
					md->DiffuseTexture += path[j];
				}
			}

			//Specular Texture
			for (int i = 0; i < surface->Specular.GetSrcObjectCount(FbxFileTexture::ClassId); i++)
			{
				FbxFileTexture* t = (FbxFileTexture*)surface->Specular.GetSrcObject(FbxTexture::ClassId, i);
				string path = ParseTexture(t);
				for (int j = 0; j < path.length(); j++)
				{
					md->SpecularTexture += path[j];
				}
			}
			mParent->AddMaterial(md);
		}
	}
}
string MaterialParser::ParseTexture(FbxFileTexture* pTexture)
{
	string path(pTexture->GetFileName());
	int start = path.find_last_of('\\');
	path = path.substr(start+1, path.size()-start-1);
	return path;
}