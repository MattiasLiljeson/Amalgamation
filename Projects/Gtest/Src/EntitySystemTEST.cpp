#include <gtest/gtest.h>
#include <bitset>

#include "EntitySystem.h"

using namespace std;

class TestSystem : public EntitySystem
{
public:
	TestSystem()
		: EntitySystem( SystemType::TestSystem,
		0,ComponentType::NON_EXISTING)
	{}
};

class TestSystem2 : public EntitySystem
{
public:
	TestSystem2()
		: EntitySystem( SystemType::TestSystem,
		1,ComponentType::NON_EXISTING)
	{}
};

TEST(EntitySystemConstructor, SimpleCreation)
{
	SystemType type = SystemType::getTypeFor(SystemType::NON_EXISTING);
	EntitySystem system( SystemType::NON_EXISTING, 1, ComponentType::Transform);
}

TEST(EntitySystemConstructor, MultipleComponentTypes)
{
	EntitySystem system(SystemType::NON_EXISTING, 3,
		ComponentType::Input,
		ComponentType::Transform,
		ComponentType::Render);

	bitset<ComponentType::NUM_COMPONENT_TYPES> set;
	set = system.getComponentBits();

	EXPECT_EQ(bitset<ComponentType::NUM_COMPONENT_TYPES>("0111"), set);
}

TEST(EntitySystemSubClassing, SimpleDerivation)
{
	SystemType type = SystemType::getTypeFor(SystemType::TestSystem);

	TestSystem* system = new TestSystem();
	system->initialize();
	system->process();
	delete system;
}

TEST(EntitySystemSubClassing, SimpleDerivationWithOneComponent)
{
	SystemType type = SystemType::getTypeFor(SystemType::TestSystem2);

	TestSystem2* system = new TestSystem2();
	system->initialize();
	system->process();
	delete system;
}