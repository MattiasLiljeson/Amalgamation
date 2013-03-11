#include "AxisRotationSystem.h"
#include "Transform.h"
#include "AxisRotate.h"
#include "MeshOffsetTransform.h"

AxisRotationSystem::AxisRotationSystem()
	: EntitySystem(SystemType::AxisRotationSystem, 3, ComponentType::Transform,
	ComponentType::MeshOffsetTransform, ComponentType::AxisRotate)
{
}

void AxisRotationSystem::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		Transform* transform = static_cast<Transform*>(p_entities[i]->getComponent(
			ComponentType::Transform));
		AxisRotate* axisRotate = static_cast<AxisRotate*>(p_entities[i]->getComponent(
			ComponentType::AxisRotate));
		MeshOffsetTransform* offset = static_cast<MeshOffsetTransform*>(p_entities[i]->
			getComponent(ComponentType::MeshOffsetTransform));
		axisRotate->angle += axisRotate->angularVelocity * m_world->getDelta();
		AglVector3 rotated = AglVector3::rotateAroundAxis(axisRotate->startVector,
			axisRotate->axis, axisRotate->angle);
		AglQuaternion quaternion = AglQuaternion::constructFromAxisAndAngle(axisRotate->axis, axisRotate->angle);
		transform->setRotation( quaternion * axisRotate->originRotation/* * offset->offset.inverse().GetRotation() */);
	}
}