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
	float t = m_world->getDelta();//m_world->getElapsedTime();

	for( unsigned int i=0; i<p_entities.size(); i++ )
	{
		InterpolationComponent2* inter = static_cast<InterpolationComponent2*>(
				p_entities[i]->getComponent(ComponentType::InterpolationComponent2));

		Transform* trans = static_cast<Transform*>(p_entities[i]->getComponent((ComponentType::Transform)));

		inter->t += t;

		while (inter->data.size() > 1 && inter->t > inter->data[1].t)
		{
			inter->data.pop_front();
		}

		if (inter->data.size() > 1)
		{
			inter->diff = inter->data[1].transform - inter->data[0].transform;
			inter->diffT = inter->data[1].t - inter->data[0].t;
			float frac = (inter->t - inter->data[0].t) / (inter->data[1].t - inter->data[0].t);

			AglMatrix transform = inter->data[0].transform * (1.0f - frac) + inter->data[1].transform*frac; 
			trans->setMatrix(transform);
		}
		else if (inter->data.size() == 1)
		{
			trans->setMatrix(inter->data[0].transform);
		}
	}
}

void InterpolationSystem2::initialize()
{

}
