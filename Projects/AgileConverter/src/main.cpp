#include "FBXParser.h"
#include "AGLExporter.h"
#include "AGLTester.h"
#include <conio.h>
#include <Windows.h>
#include <direct.h>

#define _CRTDBG_MAP_ALLOC

struct AGLData
{
	string OriginalPath;
	vector<MeshData*> Meshes; 
	vector<NodeData*> Nodes;
	vector<SkeletonData*> Skeletons;
	vector<AglSkeletonMapping*> MeshSkeletonMappings;
	vector<NodeAnimationData*> NodeAnimations;
	vector<AnimationLayerData*> AnimationLayers;
	vector<AnimationData*> Animations;
	vector<MaterialData*> Materials;
	vector<AglMaterialMapping*> MaterialMappings;
};

vector<string> FindFolders(string pFolder)
{	
	wstring folderw(pFolder.length(), L'');
	for (unsigned int i = 0; i < pFolder.size(); i++)
		folderw[i] = pFolder[i];

	folderw += L"\\*";
	WIN32_FIND_DATAW file;
	//HANDLE folder = FindFirstFile(L"ToConvert\\*.fbx*",&file);
	HANDLE folder = FindFirstFileW(folderw.c_str(),&file);

	vector<string> files;
	if(folder != INVALID_HANDLE_VALUE) 
	{
		do
		{
			if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				char* nPtr = new char[lstrlenW(file.cFileName) + 1];

				for (int i = 0; i < lstrlenW(file.cFileName); i++)
				{
					nPtr[i] = char(file.cFileName[i]);
				}

				nPtr[lstrlenW(file.cFileName)] = '\0';

				files.push_back(string(nPtr));
				delete[] nPtr;
			}

		} while(FindNextFileW(folder, &file));
	} 
	else 
	{
		cout << "Error: No such folder." << endl;
	}
	
	FindClose(folder);
	
	return files;
}

vector<string> Find(string pFolder)
{	
	wstring folderw(pFolder.length(), L'');
	for (unsigned int i = 0; i < pFolder.size(); i++)
		folderw[i] = pFolder[i];

	folderw += L"\\*.fbx*";

	WIN32_FIND_DATAW file;
	//HANDLE folder = FindFirstFile(L"ToConvert\\*.fbx*",&file);
	HANDLE folder = FindFirstFileW(folderw.c_str(),&file);
	
	vector<string> files;
	if(folder != INVALID_HANDLE_VALUE) 
	{
		do
		{
			char* nPtr = new char[lstrlenW(file.cFileName) + 1];

			for (int i = 0; i < lstrlenW(file.cFileName); i++)
			{
				nPtr[i] = char(file.cFileName[i]);
			}

			nPtr[lstrlenW(file.cFileName)] = '\0';

			files.push_back(string(nPtr));
			delete[] nPtr;

		} while(FindNextFileW(folder, &file));
	} 
	else 
	{
		cout << "Error: No such folder." << endl;
	}
	
	FindClose(folder);
	
	FindFolders(pFolder);
	return files;
}
void Save(string pFolder, vector<AGLData>& pData)
{
	char* fold;
	fold = new char[pFolder.length() + 1];
	strcpy(fold, pFolder.c_str());

	_mkdir(fold);
	delete[] fold;

	for (unsigned int i = 0; i < pData.size(); i++)
	{
		string path = pData[i].OriginalPath;
		int ind = path.find_last_of('.');
		path = path.substr(0, ind);
		ind = path.find_last_of('\\');
		path = path.substr(ind+1, path.size() - ind-1);
		path += ".agl";
		path = pFolder + '\\' + path;

		AGLExporter writer(path);
		for (unsigned int j = 0; j < pData[i].Meshes.size(); j++)
		{
			writer.AddMesh(pData[i].Meshes[j]);
		}
		for (unsigned int j = 0; j < pData[i].Nodes.size(); j++)
		{
			writer.AddNode(pData[i].Nodes[j]);
		}
		for (unsigned int j = 0; j < pData[i].Skeletons.size(); j++)
		{
			writer.AddSkeleton(pData[i].Skeletons[j]);
		}
		for (unsigned int j = 0; j < pData[i].MeshSkeletonMappings.size(); j++)
		{
			writer.AddMeshSkeletonMapping(pData[i].MeshSkeletonMappings[j]);
		}		
		for (unsigned int j = 0; j < pData[i].NodeAnimations.size(); j++)
		{
			writer.AddNodeAnimation(pData[i].NodeAnimations[j]);
		}
		for (unsigned int j = 0; j < pData[i].AnimationLayers.size(); j++)
		{
			writer.AddAnimationLayer(pData[i].AnimationLayers[j]);
		}
		for (unsigned int j = 0; j < pData[i].Animations.size(); j++)
		{
			writer.AddAnimation(pData[i].Animations[j]);
		}
		for (unsigned int j = 0; j < pData[i].Materials.size(); j++)
		{
			writer.AddMaterial(pData[i].Materials[j]);
		}
		for (unsigned int j = 0; j < pData[i].MaterialMappings.size(); j++)
		{
			writer.AddMaterialMapping(pData[i].MaterialMappings[j]);
		}

		writer.Write();

		AGLTester tester(path);
		tester.PerformTest();
	}

}

int main()
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	bool manual = false;
	string choice;
	while (true)
	{
		if (manual)
			cout << "Manual Conversion";
		else
			cout << "Automatic Conversion";
		cout << endl << "1: Convert File" << endl;
		if (manual)
			cout << "2: Turn on automatic conversion";
		else
			cout << "2: Turn on manual conversion";
		cout << endl << "3: Quit" << endl << endl;
		cout << "Choice: ";

		getline(cin, choice);

		if (choice == "1")
		{
			if (manual)
			{
				/*string path;
				cout << "Enter Path: ";
				getline(cin, path);
				FBXParser p(path);
				p.Parse();


				AGLExporter exp("test2.agl");

				vector<MeshData*> meshes = p.GetMeshes();
				for (unsigned int i = 0; i < meshes.size(); i++)
					exp.AddMesh(meshes[i]);
				exp.Write();

				cout << endl << endl;
				AGLTester tester("test2.agl");
				tester.PerformTest();*/
			}
			else
			{
				vector<string> files = Find("ToConvert");
				vector<AGLData> data;
				vector<FBXParser*> parsers;
				for (unsigned int i = 0; i < files.size(); i++)
				{
					cout << endl << "Converting: " << files[i];

					parsers.push_back(new FBXParser("ToConvert\\" + files[i]));
					parsers.back()->Parse();
					
					AGLData d;
					d.OriginalPath = "ToConvert\\" + files[i];
					vector<MeshData*> meshes = parsers.back()->GetMeshes();
					for (unsigned int j = 0; j < meshes.size(); j++)
						d.Meshes.push_back(meshes[j]);

					vector<NodeData*> nodes = parsers.back()->GetNodes();
					for (unsigned int j = 0; j < nodes.size(); j++)
						d.Nodes.push_back(nodes[j]);
					
					vector<SkeletonData*> skeletons = parsers.back()->GetSkeletons();
					for (unsigned int j = 0; j < skeletons.size(); j++)
						d.Skeletons.push_back(skeletons[j]);

					vector<AglSkeletonMapping*> msmapping = parsers.back()->GetMeshSkeletonMappings();
					for (unsigned int j = 0; j < msmapping.size(); j++)
						d.MeshSkeletonMappings.push_back(msmapping[j]);

					vector<NodeAnimationData*> nads = parsers.back()->GetNodeAnimations();
					for (unsigned int j = 0; j < nads.size(); j++)
						d.NodeAnimations.push_back(nads[j]);

					vector<AnimationLayerData*> layers = parsers.back()->GetAnimationLayers();
					for (unsigned int j = 0; j < layers.size(); j++)
						d.AnimationLayers.push_back(layers[j]);

					vector<AnimationData*> anims = parsers.back()->GetAnimations();
					for (unsigned int j = 0; j < anims.size(); j++)
						d.Animations.push_back(anims[j]);
					
					vector<MaterialData*> materials = parsers.back()->GetMaterials();
					for (unsigned int j = 0; j < materials.size(); j++)
						d.Materials.push_back(materials[j]);
					
					vector<AglMaterialMapping*> materialmappings = parsers.back()->GetMaterialMappings();
					for (unsigned int j = 0; j < materialmappings.size(); j++)
						d.MaterialMappings.push_back(materialmappings[j]);

					data.push_back(d);

					cout << endl << "Success." << endl;

				}
				Save("Converted", data);

				for (unsigned int i = 0; i < parsers.size(); i++)
					delete parsers[i];
				cout << endl << "Conversion Complete.";
			}

			cin.get();
		}
		else if (choice == "2")
		{
			manual = !manual;
		}
		else if (choice == "3")
		{
			break;
		}

		system("cls");
	}
	FBXManager::Destroy();
	return 0;
}