#include <gtest\gtest.h>
#include "Vector3.h"

class Vector3TestPositiveVectors : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		m_vectorA = Vector3(1.0f, 1.0f, 1.0f);
		m_vectorB = Vector3(0, 1.0f, 2.0f);
	}

	virtual void TearDown() {}

	Vector3 m_vectorA;
	Vector3 m_vectorB;

};

class Vector3TestNegativeVectors : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		m_vectorA = Vector3(-1.0f, -1.0f, -1.0f);
		m_vectorB = Vector3(0, -1.0f, -2.0f);
	}

	virtual void TearDown() {}

	Vector3 m_vectorA;
	Vector3 m_vectorB;

};


// Addition operator.

TEST_F(Vector3TestPositiveVectors, AdditionOperator)
{
	Vector3 addition;
	addition = m_vectorA + m_vectorB;

	EXPECT_FLOAT_EQ(1.0f, addition.x);
	EXPECT_FLOAT_EQ(2.0f, addition.y);
	EXPECT_FLOAT_EQ(3.0f, addition.z);
}

TEST_F(Vector3TestNegativeVectors, AdditionOperator)
{
	Vector3 addition;
	addition = m_vectorA + m_vectorB;

	EXPECT_FLOAT_EQ(-1.0f, addition.x);
	EXPECT_FLOAT_EQ(-2.0f, addition.y);
	EXPECT_FLOAT_EQ(-3.0f, addition.z);
}

// Subtraction operator.

TEST_F(Vector3TestPositiveVectors, SubtractionOperator)
{
	Vector3 subtraction;
	subtraction = m_vectorA - m_vectorB;

	EXPECT_FLOAT_EQ(1.0f, subtraction.x);
	EXPECT_FLOAT_EQ(0, subtraction.y);
	EXPECT_FLOAT_EQ(-1.0f, subtraction.z);
}

TEST_F(Vector3TestNegativeVectors, SubtractionOperator)
{
	Vector3 subtraction;
	subtraction = m_vectorA - m_vectorB;

	EXPECT_FLOAT_EQ(-1.0f, subtraction.x);
	EXPECT_FLOAT_EQ(0, subtraction.y);
	EXPECT_FLOAT_EQ(1.0f, subtraction.z);
}

// Division with scalar
TEST_F(Vector3TestPositiveVectors, DivisionOperator)
{
	Vector3 half = m_vectorA / 2.0f;
	EXPECT_FLOAT_EQ(0.5f, half.x);
	EXPECT_FLOAT_EQ(0.5f, half.y);
	EXPECT_FLOAT_EQ(0.5f, half.z);

	half = m_vectorB / 2.0f;
	EXPECT_FLOAT_EQ(0, half.x);
	EXPECT_FLOAT_EQ(0.5f, half.y);
	EXPECT_FLOAT_EQ(1.0f, half.z);
}
TEST_F(Vector3TestNegativeVectors, DivisionOperator)
{
	Vector3 half = m_vectorA / 2.0f;
	EXPECT_FLOAT_EQ(-0.5f, half.x);
	EXPECT_FLOAT_EQ(-0.5f, half.y);
	EXPECT_FLOAT_EQ(-0.5f, half.z);

	half = m_vectorB / 2.0f;
	EXPECT_FLOAT_EQ(-0, half.x);
	EXPECT_FLOAT_EQ(-0.5f, half.y);
	EXPECT_FLOAT_EQ(-1.0f, half.z);
}

// Dot product.

TEST(Vector3TestDotProduct, DotBothWays)
{
	Vector3 A(1.0f, 3.0f, -5.0f);
	Vector3 B(4.0f, -2.0f, -1.0f);
	float product = A.dot(B);

	EXPECT_FLOAT_EQ(3.0f, product);
}

// Cross product.

TEST_F(Vector3TestPositiveVectors, CrossProduct)
{
	Vector3 result = m_vectorA.cross(m_vectorB);
	EXPECT_FLOAT_EQ(1.0f, result.x);
	EXPECT_FLOAT_EQ(-2.0f, result.y);
	EXPECT_FLOAT_EQ(1.0f, result.z);
}

TEST_F(Vector3TestNegativeVectors, CrossProduct)
{
	Vector3 result = m_vectorA.cross(m_vectorB);
	EXPECT_FLOAT_EQ(1.0f, result.x);
	EXPECT_FLOAT_EQ(-2.0f, result.y);
	EXPECT_FLOAT_EQ(1.0f, result.z);
}

// Length of vector.

TEST_F(Vector3TestPositiveVectors, VectorLength)
{
	float len = m_vectorA.length();
	EXPECT_FLOAT_EQ(sqrtf(3), len);
	len = m_vectorB.length();
	EXPECT_FLOAT_EQ(sqrtf(5), len);
}

TEST_F(Vector3TestNegativeVectors, VectorLength)
{
	float len = m_vectorA.length();
	EXPECT_FLOAT_EQ(sqrtf(3), len);
	len = m_vectorB.length();
	EXPECT_FLOAT_EQ(sqrtf(5), len);
}

// Normalizing a vector.

TEST_F(Vector3TestPositiveVectors, VectorNormalize)
{
	m_vectorA.normalize();
	float len = m_vectorA.length();
	EXPECT_FLOAT_EQ(1.0f, len);
	EXPECT_LT(m_vectorA.x, 1.0f);
	EXPECT_LT(m_vectorA.y, 1.0f);
	EXPECT_LT(m_vectorA.z, 1.0f);
	EXPECT_GT(m_vectorA.x, 0);
	EXPECT_GT(m_vectorA.y, 0);
	EXPECT_GT(m_vectorA.z, 0);
}

// Standard setups.

TEST(StandardSetups, Zero)
{
	Vector3 vectorZero = Vector3::vecZero();
	ASSERT_FLOAT_EQ(0.0f, vectorZero.x);
	ASSERT_FLOAT_EQ(0.0f, vectorZero.y);
	ASSERT_FLOAT_EQ(0.0f, vectorZero.z);

	vectorZero.x = 1.0f;
	EXPECT_FLOAT_EQ(1.0f, vectorZero.x);
	EXPECT_FLOAT_EQ(0.0f, vectorZero.y);
	EXPECT_FLOAT_EQ(0.0f, vectorZero.z);
}

TEST(StandardSetups, Vector3Max)
{
	Vector3 vectorMax = Vector3::vecMax();
	ASSERT_FLOAT_EQ(FLT_MAX, vectorMax.x);
	ASSERT_FLOAT_EQ(FLT_MAX, vectorMax.y);
	ASSERT_FLOAT_EQ(FLT_MAX, vectorMax.z);

	vectorMax.x = 1.0f;
	EXPECT_FLOAT_EQ(1.0f, vectorMax.x);
	EXPECT_FLOAT_EQ(FLT_MAX, vectorMax.y);
	EXPECT_FLOAT_EQ(FLT_MAX, vectorMax.z);
}

TEST(StandardSetups, Vector3Min)
{
	Vector3 vectorMin = Vector3::vecMin();
	ASSERT_FLOAT_EQ(FLT_MIN, vectorMin.x);
	ASSERT_FLOAT_EQ(FLT_MIN, vectorMin.y);
	ASSERT_FLOAT_EQ(FLT_MIN, vectorMin.z);

	vectorMin.x = 1.0f;
	EXPECT_FLOAT_EQ(1.0f, vectorMin.x);
	EXPECT_FLOAT_EQ(FLT_MIN, vectorMin.y);
	EXPECT_FLOAT_EQ(FLT_MIN, vectorMin.z);
}