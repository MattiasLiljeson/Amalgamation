#pragma once

#include <Component.h>

///
/// TAG
///
class Tag : public Component
{
public:
	Tag();

	virtual ~Tag();
};

///
/// ShipFlyMode_TAG
///
class ShipFlyMode_TAG : public Tag {};

///
/// ShipFlyMode_TAG
///
class ShipEditMode_TAG : public Tag {};

///
/// ShipFollowMode_TAG
///
class CameraFollowMode_TAG : public Tag {};

///
/// ShipFlyMode_TAG
///
class CameraOrbitMode_TAG : public Tag {};
