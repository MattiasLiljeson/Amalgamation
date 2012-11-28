#include <gtest/gtest.h>
#include <bitset>
#include "ComponentType.h"

using namespace std;

TEST(ComponentTypeGetId, OneComponentType)
{
	ComponentType ct;
	int ct_id;


	ct = ComponentType::getTypeFor(ComponentType::Position);
	ct_id = ct.getId();
	EXPECT_EQ( 0, ct_id );

	ct = ComponentType::getTypeFor(ComponentType::Position);
	ct_id = ct.getId();
	EXPECT_EQ( 0, ct_id );

	ct = ComponentType::getTypeFor(ComponentType::Position);
	ct_id = ct.getId();
	EXPECT_EQ( 0, ct_id );
}

TEST(ComponentTypeGetId, SeveralComponentTypes)
{
	ComponentType ct;
	int ct_id;


	ct = ComponentType::getTypeFor(ComponentType::Position);
	ct_id = ct.getId();
	EXPECT_EQ( 0, ct_id );

	ct = ComponentType::getTypeFor(ComponentType::Orientation);
	ct_id = ct.getId();
	EXPECT_EQ( 1, ct_id );

	ct = ComponentType::getTypeFor(ComponentType::Render);
	ct_id = ct.getId();
	EXPECT_EQ( 2, ct_id );

	ct = ComponentType::getTypeFor(ComponentType::Input);
	ct_id = ct.getId();
	EXPECT_EQ( 3, ct_id );
}

TEST(ComponentTypeGetId, NonExisting)
{
	ComponentType ct;
	int ct_id;


	ct = ComponentType::getTypeFor(ComponentType::NON_EXISTING);
	ct_id = ct.getId();
	EXPECT_EQ( -1, ct_id );
}

TEST(ComponentTypeGetBit, SeveralComponentTypes)
{
	ComponentType ct;
	bitset<ComponentType::NUM_COMPONENT_TYPES> ct_set;


	ct = ComponentType::getTypeFor(ComponentType::Position);
	ct_set = ct.getBit();
	EXPECT_EQ( bitset<ComponentType::NUM_COMPONENT_TYPES>("0001"), ct_set );

	ct = ComponentType::getTypeFor(ComponentType::Orientation);
	ct_set = ct.getBit();
	EXPECT_EQ( bitset<ComponentType::NUM_COMPONENT_TYPES>("0010"), ct_set );

	ct = ComponentType::getTypeFor(ComponentType::Render);
	ct_set = ct.getBit();
	EXPECT_EQ( bitset<ComponentType::NUM_COMPONENT_TYPES>("0100"), ct_set );

	ct = ComponentType::getTypeFor(ComponentType::Input);
	ct_set = ct.getBit();
	EXPECT_EQ( bitset<ComponentType::NUM_COMPONENT_TYPES>("1000"), ct_set );
}

TEST(ComponentTypeGetBit, RandomOrder)
{
	ComponentType ct;
	bitset<ComponentType::NUM_COMPONENT_TYPES> ct_set;


	ct = ComponentType::getTypeFor(ComponentType::Input);
	ct_set = ct.getBit();
	EXPECT_EQ( bitset<ComponentType::NUM_COMPONENT_TYPES>("1000"), ct_set );

	ct = ComponentType::getTypeFor(ComponentType::Input);
	ct_set = ct.getBit();
	EXPECT_EQ( bitset<ComponentType::NUM_COMPONENT_TYPES>("1000"), ct_set );

	ct = ComponentType::getTypeFor(ComponentType::Input);
	ct_set = ct.getBit();
	EXPECT_EQ( bitset<ComponentType::NUM_COMPONENT_TYPES>("1000"), ct_set );

	ct = ComponentType::getTypeFor(ComponentType::Input);
	ct_set = ct.getBit();
	EXPECT_EQ( bitset<ComponentType::NUM_COMPONENT_TYPES>("1000"), ct_set );

	ct = ComponentType::getTypeFor(ComponentType::Orientation);
	ct_set = ct.getBit();
	EXPECT_EQ( bitset<ComponentType::NUM_COMPONENT_TYPES>("0010"), ct_set );

	ct = ComponentType::getTypeFor(ComponentType::Position);
	ct_set = ct.getBit();
	EXPECT_EQ( bitset<ComponentType::NUM_COMPONENT_TYPES>("0001"), ct_set );

	ct = ComponentType::getTypeFor(ComponentType::Render);
	ct_set = ct.getBit();
	EXPECT_EQ( bitset<ComponentType::NUM_COMPONENT_TYPES>("0100"), ct_set );

	ct = ComponentType::getTypeFor(ComponentType::Render);
	ct_set = ct.getBit();
	EXPECT_EQ( bitset<ComponentType::NUM_COMPONENT_TYPES>("0100"), ct_set );

	ct = ComponentType::getTypeFor(ComponentType::Render);
	ct_set = ct.getBit();
	EXPECT_EQ( bitset<ComponentType::NUM_COMPONENT_TYPES>("0100"), ct_set );
}