#include "AglSkeleton.h"
#include "AglScene.h"

AglSkeleton::AglSkeleton(AglSkeletonHeader p_header, AglJoint* p_joints, AglScene* p_scene)
{
	m_header = p_header;
	m_joints = p_joints;
	m_scene = p_scene;
}
AglSkeleton::~AglSkeleton()
{
	delete[] m_joints;
}

AglSkeletonHeader AglSkeleton::getHeader()
{
	return m_header;
}
AglJoint* AglSkeleton::getJoints()
{
	return m_joints;
}
AglJoint* AglSkeleton::getRoot()
{
	AglJoint* j = NULL;
	int ind = -1;
	for (int i = 0; i < m_header.jointCount; i++)
	{
		if (m_joints[i].parentIndex < 0)
		{
			j = &m_joints[i];
			ind = i;
			break;
		}
	}
	return j;
}

AglMatrix AglSkeleton::getGlobalTransform(int p_joint)
{
	AglJoint j = m_joints[p_joint];

	AglMatrix transform;
	if (m_scene->getDynamicNode(j.nodeID).animated)
		transform = m_scene->getDynamicNode(j.nodeID).transform;
	else
		transform = m_scene->getNode(j.nodeID).localTransform;

	if (j.parentIndex >= 0)
		return transform * getGlobalTransform(j.parentIndex);
	return transform * m_header.baseTransform;
}
AglMatrix AglSkeleton::getGlobalTransform(int p_joint, float p_time)
{
	int k = 0;
	k = 1 / k;
	//INCORRECT!
	//AglAnimation* anim = m_scene->getAnimation(0);
	//return anim->evaluate(p_time, m_joints[p_joint].nodeID);
	return AglMatrix::identityMatrix();
}

AglMatrix AglSkeleton::getInverseBindMatrix(int p_joint)
{
	AglJoint j = m_joints[p_joint];
	AglNode n = m_scene->getNode(j.nodeID);
	return n.inverseBindMatrix;
}