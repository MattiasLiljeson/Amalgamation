#include <gtest/gtest.h>

#include "EntityManager.h"

TEST(EntityCreation, WithoutComponents)
{
	EntityManager entityManager;

	Entity* entity = NULL;
	entity = entityManager.createEntityInstance();

	EXPECT_EQ( 0, entity->getIndex() );

	delete entity;
}