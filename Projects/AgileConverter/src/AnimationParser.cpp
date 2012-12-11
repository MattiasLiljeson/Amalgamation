#include "AnimationParser.h"
#include "FBXParser.h"

AnimationParser::AnimationParser(FBXParser* pParent)
{
	mParent = pParent;
	GetStacks();
	GetLayers();
	counter = 0;
}

void AnimationParser::GetStacks()
{
	for (int i = 0; i < mParent->GetScene()->GetSrcObjectCount(FBX_TYPE(FbxAnimStack)); i++)
    {
		FbxAnimStack* animStack = FbxCast<FbxAnimStack>(mParent->GetScene()->GetSrcObject(FBX_TYPE(FbxAnimStack), i));
        const char* name = animStack->GetName();

		AnimationData* anim = new AnimationData();
		anim->Source = animStack;
		mAnimations.push_back(anim);
		mParent->AddAnimation(anim);
	}
}
void AnimationParser::GetLayers()
{
	for (int i = 0; i < (int)mAnimations.size(); i++)
	{
		int animLayers = mAnimations[i]->Source->GetMemberCount(FBX_TYPE(FbxAnimLayer));
		for (int j = 0; j < animLayers; j++)
		{
			FbxAnimLayer* animLayer = mAnimations[i]->Source->GetMember(FBX_TYPE(FbxAnimLayer), j);

			int ind = mParent->FindIndex(animLayer);

			if (ind == -1)
			{
				const char* name = animLayer->GetName();
				AnimationLayerData* layer = new AnimationLayerData();
				layer->Source = animLayer;
				mParent->AddAnimationLayer(layer);
				mAnimations[i]->Layers.push_back(layer->ID);
				mAnimationLayers.push_back(layer);
			}
			else
				mAnimations[i]->Layers.push_back(ind);
		}
	}
}

void AnimationParser::Parse(vector<NodeData*> pNodes)
{
	for (unsigned int i = 0; i < pNodes.size(); i++)
	{
		cout << "\nCreating Animations for node " << pNodes[i]->SourceNode->GetName() << "----- Size: " << (16*sizeof(float) * counter) / 1000000.0f;
		Parse(pNodes[i]);
	}
}
void AnimationParser::Parse(NodeData* pNode)
{
	/*for (unsigned int j = 0; j < mAnimationLayers.size(); j++)
	{
		vector<AglKeyFrame> frames;

		FbxTimeSpan span;
		FbxTime start(0);
		FbxTime stop(0);

		vector<vector<FbxTime>> times;
		FbxNode* node = pNode->SourceNode;
		FbxAnimLayer* pAnimLayer = mAnimationLayers[j]->Source;
		node->GetAnimationInterval(span);
		start = span.GetStart();
		stop = span.GetStop();

		unsigned long curr = (unsigned long)start.GetMilliSeconds();
		unsigned long end = (unsigned long)stop.GetMilliSeconds();
		//60 frames per second based time step
		unsigned long step = 33;

		while (curr < end)
		{
			float currt = 0.001f * curr;
			FbxTime time((FbxLongLong)(46186158000 * currt));
			FbxAMatrix mat = pNode->SourceNode->EvaluateLocalTransform(time);
			FbxVector4 row1 = mat.GetRow(0);
			FbxVector4 row2 = mat.GetRow(1);
			FbxVector4 row3 = mat.GetRow(2);
			FbxVector4 row4 = mat.GetRow(3);
			float matdata[] = { (float)row1.mData[0], (float)row1.mData[1], (float)row1.mData[2], (float)row1.mData[3],
								(float)row2.mData[0], (float)row2.mData[1], (float)row2.mData[2], (float)row2.mData[3],
								(float)row3.mData[0], (float)row3.mData[1], (float)row3.mData[2], (float)row3.mData[3],
								(float)row4.mData[0], (float)row4.mData[1], (float)row4.mData[2], (float)row4.mData[3] };


			AglKeyFrame frame;
			memcpy(frame.Transform, matdata, sizeof(float) * 16);
			frame.Time = curr * 0.001f;
			frames.push_back(frame);

			curr += step;

			counter++;
		}
		NodeAnimationData* nad = new NodeAnimationData();
		nad->KeyFrames = frames;
		nad->NodeID = pNode->ID;
		mParent->AddNodeAnimation(nad);
		mNodeAnimations.push_back(nad);
		mAnimationLayers[j]->NodeAnimations.push_back(nad->ID);
	}*/


	for (unsigned int j = 0; j < mAnimationLayers.size(); j++)
	{
		vector<AglKeyFrame> frames;
		const char* comp[3];
		comp[0] = FBXSDK_CURVENODE_COMPONENT_X;
		comp[1] = FBXSDK_CURVENODE_COMPONENT_Y;
		comp[2] = FBXSDK_CURVENODE_COMPONENT_Z;
		FbxAnimCurve* lAnimCurve;
		vector<FbxTime> times;
		for (int c = 0; c < 3; c++)
		{
			lAnimCurve = pNode->SourceNode->LclTranslation.GetCurve(mAnimationLayers[j]->Source, comp[c]);
			if (lAnimCurve)
			{
				int count = lAnimCurve->KeyGetCount();
				for (int k = 0; k < lAnimCurve->KeyGetCount(); k++)
				{
					//Add the key
					FbxTime time = lAnimCurve->KeyGetTime(k);
					if (!HasKey(times, time))
						InsertKey(times, time);
				}
			}
			lAnimCurve = pNode->SourceNode->LclRotation.GetCurve(mAnimationLayers[j]->Source, comp[c]);
			if (lAnimCurve)
			{
				int count = lAnimCurve->KeyGetCount();
				for (int k = 0; k < lAnimCurve->KeyGetCount(); k++)
				{
					//Add the key
					FbxTime time = lAnimCurve->KeyGetTime(k);
					if (!HasKey(times, time))
					{
						InsertKey(times, time);
					}
				}
			}
			lAnimCurve = pNode->SourceNode->LclScaling.GetCurve(mAnimationLayers[j]->Source, comp[c]);
			if (lAnimCurve)
			{
				int count = lAnimCurve->KeyGetCount();
				for (int k = 0; k < lAnimCurve->KeyGetCount(); k++)
				{
					//Add the key
					FbxTime time = lAnimCurve->KeyGetTime(k);
					if (!HasKey(times, time))
						InsertKey(times, time);
				}
			}    
		}
		/*if (times.size() == 0)
		{
			FbxTimeSpan span;
			FbxTime start(0);
			FbxTime stop(0);
			FbxNode* node = pNode->SourceNode;
			FbxAnimLayer* pAnimLayer = mAnimationLayers[j]->Source;
			node->GetAnimationInterval(span);
			start = span.GetStart();
			stop = span.GetStop();
			times.push_back(start);
			times.push_back(stop);
			
		}*/
		for (int k = 0; k < times.size(); k++)
		{
			//float currt = (float)((unsigned long)(times[k].GetMilliSeconds()) * 0.001f);
			float currt = 0.001f * times[k].GetMilliSeconds();
			FbxAMatrix mat = pNode->SourceNode->EvaluateLocalTransform(times[k]);
			//FbxAMatrix mat = pNode->SourceNode->EvaluateLocalTransform((FbxLongLong)(46186158000 * currt));
			FbxVector4 row1 = mat.GetRow(0);
			FbxVector4 row2 = mat.GetRow(1);
			FbxVector4 row3 = mat.GetRow(2);
			FbxVector4 row4 = mat.GetRow(3);
			float matdata[] = { (float)row1.mData[0], (float)row1.mData[1], (float)row1.mData[2], (float)row1.mData[3],
								(float)row2.mData[0], (float)row2.mData[1], (float)row2.mData[2], (float)row2.mData[3],
								(float)row3.mData[0], (float)row3.mData[1], (float)row3.mData[2], (float)row3.mData[3],
								(float)row4.mData[0], (float)row4.mData[1], (float)row4.mData[2], (float)row4.mData[3] };


			AglKeyFrame frame;
			frame.transform = AglMatrix(matdata);
			frame.time = currt;
			frames.push_back(frame);

			counter++;
		}
		if (frames.size() > 0)
		{
			NodeAnimationData* nad = new NodeAnimationData();
			nad->KeyFrames = frames;
			nad->NodeID = pNode->ID;
			mParent->AddNodeAnimation(nad);
			mNodeAnimations.push_back(nad);
			mAnimationLayers[j]->NodeAnimations.push_back(nad->ID);
		}
	}
}
bool AnimationParser::HasKey(vector<FbxTime>& pKeys, FbxTime pKey)
{
	int min = 0; 
	int max = pKeys.size() - 1;
	while (max >= min)
	{
		int curr = 0.5f * (min + max);
		if (pKeys[curr] == pKey)
			return true;
		else if (pKey < pKeys[curr])
			max = curr - 1;
		else
			min = curr + 1;
	}
	return false;
}
void AnimationParser::InsertKey(vector<FbxTime>& pKeys, FbxTime pKey)
{
	int ind = pKeys.size();
	pKeys.push_back(pKey);
	while (ind > 0 && pKeys[ind] < pKeys[ind-1])
	{
		FbxTime temp(pKeys[ind]);
		pKeys[ind] = FbxTime(pKeys[ind-1]);
		pKeys[ind-1] = FbxTime(temp);
		ind--;
	}
}