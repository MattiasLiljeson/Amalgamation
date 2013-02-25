#include "SkinParser.h"
#include "FBXParser.h"

SkinParser::SkinParser(FBXParser* pParent, FbxGeometry* pGeometry)
{
	mParent = pParent;
	mGeometry = pGeometry;
}
void SkinParser::Parse()
{
	int skincount = mGeometry->GetDeformerCount(FbxDeformer::eSkin);
	for (int i = 0; i < skincount; i++)
	{
		ParseSkin((FbxSkin*)mGeometry->GetDeformer(i, FbxDeformer::eSkin));
	}
	//CreateJointHierarchy();
}
void SkinParser::ParseSkin(FbxSkin* pSkin)
{
	SkeletonData* d = new SkeletonData();
	d->Source = pSkin;
	d->ID = -1;
	d->Name = pSkin->GetName();

	//The number of clusters in a certain skin.
	//A cluster is a joint like an elbow or a shoulder
	int clusters = pSkin->GetClusterCount();
    for (int i = 0; i < clusters; i++)
    {
        FbxCluster* c = pSkin->GetCluster(i);  

		FbxCluster::ELinkMode lm = c->GetLinkMode();

		char* name = "";
		const char* parent = "";
		FbxNode* parentNode = NULL;
		FbxNode* node = NULL;;
		float	InverseBindMatrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
		float	LocalTransform[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        if(c->GetLink() != NULL)
        {
			node = c->GetLink();
			name = (char *)c->GetLink()->GetName();
			parentNode = c->GetLink()->GetParent();
			if (parentNode)
			{
				parent = parentNode->GetName();
			}

			//Find the inverse bind pose matrix of the cluster
			FbxAMatrix transform;
			c->GetTransformMatrix(transform);
			FbxAMatrix transformLink;
			c->GetTransformLinkMatrix(transformLink);
			FbxAMatrix InverseBindPose = transformLink.Inverse() * transform;

			for (int row = 0; row < 4; row++)
				for (int column = 0; column < 4; column++)
					InverseBindMatrix[row*4 + column] = (float)InverseBindPose.Get(row, column);

			//Find the default local transform of the cluster
			transform = c->GetLink()->EvaluateLocalTransform();
			for (int row = 0; row < 4; row++)
				for (int column = 0; column < 4; column++)
					LocalTransform[row*4 + column] = (float)transform.Get(row, column);
        }
		else
		{
			//Serious Error which should not occur.
			return;
		}

		JointData* j = new JointData();
		j->ID = d->Joints.size();
		j->NodeID = mParent->GetNodeID(node, c);
		j->node = node;
		
		FbxNodeAttribute* attr = node->GetNodeAttribute();
		FbxSkeleton* skel = (FbxSkeleton*)attr;
		j->Type = skel->GetSkeletonType();

		j->FbxParent = parentNode;
		j->Parent = -1;
		j->affectsVertices = true;
		d->Joints.push_back(j);
    }

	//Add the SkeletonData to the FBXParser
	ArrangeSkeleton(d);
	mParent->AddSkeleton(d);
}

void SkinParser::ArrangeSkeleton(SkeletonData* pSkeleton)
{
	vector<JointData*>& joints = pSkeleton->Joints;

	int sizebeforeadded = joints.size();

	bool docheck = true;
	while (docheck)
	{
		docheck = false;
		for (unsigned int i = 0; i < joints.size(); i++)
		{
			for (unsigned int j = 0; j < joints.size(); j++)
			{
				if (joints[i]->FbxParent == mParent->GetNode(joints[j]->NodeID)->SourceNode && i != j)
				{
					joints[i]->Parent = j;
				}
			}
			//Add non-cluster nodes to the hierarchy
			//these nodes do not affect any positions
			//but are still part of the skeleton
			if (joints[i]->Parent == -1 && joints[i]->FbxParent)
			{
				//New Nested if
				FbxNodeAttribute* na = joints[i]->FbxParent->GetNodeAttribute();
				const char* name = joints[i]->FbxParent->GetName();
				if (na && na->GetAttributeType() == FbxNodeAttribute::eSkeleton)
				{
					docheck = true;
				
					FbxNode* newJoint = joints[i]->FbxParent;
				
					FbxNode* parent = newJoint->GetParent();

					JointData* j = new JointData();
					j->ID = joints.size();
					j->NodeID = mParent->GetNodeID(newJoint);
					j->FbxParent = parent;
					j->Parent = -1;
					j->affectsVertices = false;
					j->node = joints[i]->FbxParent;
					FbxNodeAttribute* attr = newJoint->GetNodeAttribute();
					FbxSkeleton* skel = (FbxSkeleton*)attr;
					j->Type = skel->GetSkeletonType();
					joints.push_back(j);
				}
			}
		}
	}
	
	return;
}