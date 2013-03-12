#include "FBXParser.h"
#include "AGLExporter.h"
#include "AGLTester.h"
#include <conio.h>
#include <Windows.h>
#include <direct.h>
#include "FileManagement.h"

#define _CRTDBG_MAP_ALLOC

void Save(AGLData& pData)
{
	string path = pData.OriginalPath;
	int ind = path.find_last_of('.');
	path = path.substr(0, ind);
	path += ".agl";

	AGLExporter writer(path);
	for (unsigned int j = 0; j < pData.Meshes.size(); j++)
	{
		writer.AddMesh(pData.Meshes[j]);
	}
	for (unsigned int j = 0; j < pData.Nodes.size(); j++)
	{
		writer.AddNode(pData.Nodes[j]);
	}
	for (unsigned int j = 0; j < pData.Skeletons.size(); j++)
	{
		writer.AddSkeleton(pData.Skeletons[j]);
	}
	for (unsigned int j = 0; j < pData.MeshSkeletonMappings.size(); j++)
	{
		writer.AddMeshSkeletonMapping(pData.MeshSkeletonMappings[j]);
	}		
	for (unsigned int j = 0; j < pData.NodeAnimations.size(); j++)
	{
		writer.AddNodeAnimation(pData.NodeAnimations[j]);
	}
	for (unsigned int j = 0; j < pData.AnimationLayers.size(); j++)
	{
		writer.AddAnimationLayer(pData.AnimationLayers[j]);
	}
	for (unsigned int j = 0; j < pData.Animations.size(); j++)
	{
		writer.AddAnimation(pData.Animations[j]);
	}
	for (unsigned int j = 0; j < pData.Materials.size(); j++)
	{
		writer.AddMaterial(pData.Materials[j]);
	}
	for (unsigned int j = 0; j < pData.MaterialMappings.size(); j++)
	{
		writer.AddMaterialMapping(pData.MaterialMappings[j]);
	}
	for (unsigned int j = 0; j < pData.CP.size(); j++)
	{
		writer.AddConnectionPoint(pData.CP[j]);
	}
	writer.Write();
}
vector<string> FindFiles(string pPath)
{	
	string thefolder = pPath;

	thefolder += "\\*.fbx*";

	WIN32_FIND_DATA file;
	HANDLE folder = FindFirstFile(thefolder.c_str(),&file);

	vector<string> files;
	if(folder != INVALID_HANDLE_VALUE) 
	{
		do
		{
			char* nPtr = new char[lstrlen(file.cFileName) + 1];

			for (int i = 0; i < lstrlen(file.cFileName); i++)
			{
				nPtr[i] = char(file.cFileName[i]);
			}

			nPtr[lstrlen(file.cFileName)] = '\0';

			files.push_back(pPath + '\\' + string(nPtr));
			delete[] nPtr;

		} while(FindNextFile(folder, &file));
	} 
	else 
	{
		cout << "Error: No such folder." << endl;
	}

	return files;
}

int main()
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	bool manual = false;
	string choice;
	vector<string> toConvert;
	while (true)
	{
		cout << "Added Files:" << endl;
		for (unsigned int i = 0; i < toConvert.size(); i++)
			cout << toConvert[i] << endl;
		cout << endl;
		cout << "1: Add File" << endl;
		cout << "2: Add Files" << endl;
		cout << "3: Convert" << endl;
		cout << "4: Quit" << endl << endl;
		cout << "Choice: ";

		getline(cin, choice);

		if (choice == "1")
		{
			string newFile = openfilename("Fbx Files (*.fbx*)\0*.fbx*\0", NULL);
			if (newFile != "")
				toConvert.push_back(newFile);
		}
		else if (choice == "2")
		{
			string path = openFolder();
			if (path != "")
			{
				vector<string> newfiles = FindFiles(path);
				for (unsigned int i = 0; i < newfiles.size(); i++)
				{
					toConvert.push_back(newfiles[i]);
				}
			}
		}
		else if (choice == "3")
		{
			for (unsigned int i = 0; i < toConvert.size(); i++)
			{
				string file = toConvert[i];
				FBXParser* parser;

				cout << endl << "Converting: " << file;

				parser = new FBXParser(file);
				parser->Parse();

				AGLData data = parser->GetData();			
				Save(data);
				delete parser;
				cout << endl << "Conversion Complete.";
			}
			toConvert.clear();
			cin.get();
		}
		else if (choice == "4")
		{
			break;
		}

		system("cls");
	}
	FBXManager::Destroy();
	return 0;
}