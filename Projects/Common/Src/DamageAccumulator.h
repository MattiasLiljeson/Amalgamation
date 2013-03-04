#pragma once

///---------------------------------------------------------------------------------------
/// \brief Struct used to accumulate damage and transfer this info to the client
///
///---------------------------------------------------------------------------------------

struct DamageAccumulator
{
	DamageAccumulator()
	{
		reset();
		latestPerp = -1;
	}

	void reset() 
	{
		accumulatedDamage = 0.0f;
	}

	void operator+=( const DamageAccumulator& rhs )
	{
		accumulatedDamage += rhs.accumulatedDamage;
		latestPerp = rhs.latestPerp;
	}

	float accumulatedDamage;
	int latestPerp;
};