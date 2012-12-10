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
	for (int i = 0; i < m_header.jointCount; i++)
	{
		if (m_joints[i].parentIndex < 0)
			return &m_joints[i];
	}
	return NULL;
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
	return transform;
}
AglMatrix AglSkeleton::getInverseBindMatrix(int p_joint)
{
	AglJoint j = m_joints[p_joint];
	AglNode n = m_scene->getNode(j.nodeID);
	return n.inverseBindMatrix;
}