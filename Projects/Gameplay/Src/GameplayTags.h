#pragma once

#include <Tag.h>

///
/// ShipFlyMode_TAG
///
class ShipFlyMode_TAG : public Tag{ public: ShipFlyMode_TAG() : Tag( ComponentType::TAG_ShipFlyMode ){}};

///
/// ShipEditMode_TAG
///
class ShipEditMode_TAG : public Tag{ public: ShipEditMode_TAG() : Tag( ComponentType::TAG_ShipEditMode ){}};

///
/// LookAtFollowMode_TAG
///
class LookAtFollowMode_TAG : public Tag{ public: LookAtFollowMode_TAG() : Tag( ComponentType::TAG_LookAtFollowMode ){}};

///
/// LookatOrbitMode_TAG
///
class LookAtOrbitMode_TAG : public Tag{ public: LookAtOrbitMode_TAG() : Tag( ComponentType::TAG_LookAtOrbitMode ){}};

///
/// Ship_TAG for server
///
class Ship_TAG : public Tag{ public: Ship_TAG() : Tag( ComponentType::TAG_Ship ){}};

///
/// MyShip_TAG for client
///
class MyShip_TAG : public Tag{ public: MyShip_TAG() : Tag( ComponentType::TAG_MyShip ){}};

class MainCamera_TAG : public Tag{ public: MainCamera_TAG() : Tag( ComponentType::TAG_MainCamera ){}};

class ShadowCamera_TAG : public Tag{ public: ShadowCamera_TAG() : Tag( ComponentType::TAG_ShadowCamera ){}};

class LookAtMainCamera_TAG : public Tag{ public: LookAtMainCamera_TAG() : Tag( ComponentType::TAG_LookAtMainCamera ){}};

class Highlight_TAG : public Tag{ public: Highlight_TAG() : Tag( ComponentType::TAG_Highlight ){}};

class PositionalSound_TAG : public Tag{ public: PositionalSound_TAG() : Tag( ComponentType::TAG_PositionalSound ){}};

class AmbientRangeSound_TAG : public Tag{ public: AmbientRangeSound_TAG() : Tag( ComponentType::TAG_AmbientRangeSound ){}};

class AddToParent_TAG : public Tag{ public: AddToParent_TAG() : Tag( ComponentType::TAG_AddToParent ){}};