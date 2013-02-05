#pragma once

#include <Tag.h>

///
/// ShipFlyMode_TAG
///
class ShipFlyMode_TAG : public Tag {};

///
/// ShipEditMode_TAG
///
class ShipEditMode_TAG : public Tag {};

///
/// LookAtFollowMode_TAG
///
class LookAtFollowMode_TAG : public Tag {};

///
/// LookatOrbitMode_TAG
///
class LookAtOrbitMode_TAG : public Tag {};

///
/// Ship_TAG for server
///
class Ship_TAG : public Tag {};

///
/// MyShip_TAG for client
///
class MyShip_TAG : public Tag {};

class MainCamera_TAG : public Tag{};

class ShadowCamera_TAG : public Tag{};