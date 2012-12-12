#include "AGLTester.h"
#include <sstream>

AGLTester::AGLTester(string pPath)
{
	mPath = new char[pPath.length()+1];
	strcpy(mPath, pPath.c_str());
	mReader = new AglReader(mPath);
}
AGLTester::~AGLTester()
{
	delete[] mPath;
}
void AGLTester::PerformTest()
{
	cout << "Inspecting file: " << mPath << endl << endl;

	//vector<AglMesh*> meshes = mReader->GetMeshes();
	//cout << "Mesh Count: " << meshes.size() << endl;
	//cout << "Material Count: " << mReader->GetScene()->GetMaterials().size() << endl;
	//cout << "Skeleton Count: " << mReader->GetSkeletons().size() << endl;
	//cout << "Animation Count: " << mReader->GetScene()->GetAnimations().size() << endl;

	/*cout << endl;
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		AglMeshHeader h = meshes[i]->GetHeader();
		stringstream ss;
		ss << i;
		cout << "Mesh " + ss.str() << endl;
		cout << "#####" << endl;
		cout << "Name: " << mReader->GetScene()->GetName(h.NameID) << endl;
		cout << "Vertex Count: " << h.VertexCount << endl;
		cout << "Index Count: " << h.IndexCount << endl;
		cout << "Vertex Format: " << h.VertexFormat << endl << endl;

	}*/

}