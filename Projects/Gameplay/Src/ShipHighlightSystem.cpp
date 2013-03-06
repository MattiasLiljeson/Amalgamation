#include "ShipHiglightSystem.h"
#include "ShipHighlight.h"
#include "GradientComponent.h"
#include "Transform.h"

ShipHighlightSystem::ShipHighlightSystem()
	: EntitySystem(SystemType::ShipHighlightSystem, 1,
	ComponentType::TAG_Ship)
{
	m_minDistance = 100;
	m_maxDistance = 300;
}

void ShipHighlightSystem::processEntities( const vector<Entity*>& p_entities )
{
	Entity* ownShip = NULL;
	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		if (p_entities[i]->getComponent(ComponentType::TAG_MyShip))
		{
			ownShip = p_entities[i];
			break;
		}
	}

	if (!ownShip)
		return;

	AglVector3 ownPos = (static_cast<Transform*>(ownShip->getComponent(ComponentType::Transform)))->getTranslation();

	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		if (p_entities[i] != ownShip)
		{
			ShipHighlight* highlight = static_cast<ShipHighlight*>(p_entities[i]->getComponent(ComponentType::ShipHighlight));
			AglVector3 pos = (static_cast<Transform*>(p_entities[i]->getComponent(ComponentType::Transform)))->getTranslation();

			if (!highlight)
			{
				highlight = new ShipHighlight();
				p_entities[i]->addComponent(ComponentType::ShipHighlight, highlight);
				p_entities[i]->applyComponentChanges();
			}

			float distance = AglVector3::length(pos-ownPos);

			if (distance > m_minDistance)
			{
				highlight->active = true;

				float strength = min((distance - m_minDistance) / (m_maxDistance - m_minDistance), 1.0f);

				GradientComponent* gradient = static_cast<GradientComponent*>(p_entities[i]->getComponent(
					ComponentType::Gradient));

				AglVector4 tone = AglVector4(1.0f, 1.0f, 1.0f, 1.0f);
				if (gradient)
				{
					tone = gradient->m_color.layerTwo;

					float maxChannel = max(tone.x, max(tone.y, tone.z));

					tone *= (1.0f / maxChannel);
					tone.w = 1.0f;

				}
				highlight->setAdditiveTone(tone*strength);
			}
			else
			{
				highlight->active = false;
			}
		}
	}
}
float* ShipHighlightSystem::getMinDistance()
{
	return &m_minDistance;
}
float* ShipHighlightSystem::getMaxDistance()
{
	return &m_maxDistance;
}