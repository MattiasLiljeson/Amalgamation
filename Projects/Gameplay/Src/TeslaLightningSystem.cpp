#include "TeslaLightningSystem.h"
#include "TeslaEffectPiece.h"
#include "Transform.h"
#include <RandomUtil.h>

TeslaLightningSystem::TeslaLightningSystem()
	: EntitySystem(SystemType::TeslaLightningSystem, 2,
	ComponentType::TeslaEffectPiece, ComponentType::Transform)
{

}

void TeslaLightningSystem::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		TeslaEffectPiece* effectPiece = static_cast<TeslaEffectPiece*>(p_entities[i]->
			getComponent(ComponentType::TeslaEffectPiece));
		effectPiece->lifeTime -= m_world->getDelta();
		if(effectPiece->lifeTime <= 0.0f)
		{
			m_world->deleteEntity(p_entities[i]);
		}
		else
		{
			Transform* pieceTransform = static_cast<Transform*>(p_entities[i]->
				getComponent(ComponentType::Transform));
			float scaleFactor = RandomUtil::randomInterval(1.0f, 5.0f);
			AglVector3 upScale = AglVector3::right() * scaleFactor;
			AglVector3 rightScale = AglVector3::forward() * scaleFactor;
			AglVector3 scale = effectPiece->forwardScale + upScale + rightScale;
			pieceTransform->setScale(scale);
		}
	}
}