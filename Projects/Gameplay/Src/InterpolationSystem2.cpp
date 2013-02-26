#include "InterpolationSystem2.h"

// Components
#include "InterpolationComponent2.h"
#include "Transform.h"
#include "CameraInfo.h"

InterpolationSystem2::InterpolationSystem2( )
	: EntitySystem( SystemType::InterpolationSystem2, 2,
	ComponentType::InterpolationComponent2, ComponentType::Transform)
{

}

InterpolationSystem2::~InterpolationSystem2()
{
}

void InterpolationSystem2::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();

	for( unsigned int i=0; i<p_entities.size(); i++ )
	{
		InterpolationComponent2* inter = static_cast<InterpolationComponent2*>(
				p_entities[i]->getComponent(ComponentType::InterpolationComponent2));

		Transform* trans = static_cast<Transform*>(p_entities[i]->getComponent((ComponentType::Transform)));

		inter->t += dt;
		float frac = 0.0f;
		if (inter->end - inter->start > 0)
			frac = (inter->t - inter->start) / (inter->end - inter->start);

		if (inter->t > inter->end)
			frac = 1.0f;

		AglMatrix mat = inter->source*(1-frac) + inter->target*frac;

		trans->setMatrix(mat);
	}
}

void InterpolationSystem2::initialize()
{

}
