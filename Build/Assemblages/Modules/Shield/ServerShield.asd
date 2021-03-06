e ServerShield

c LoadMesh
d s m_filename shield_module.agl

c Transform
d f m_translationX 0.0
d f m_translationY 0.0
d f m_translationZ 0.0
d f m_scaleX 1.0f;
d f m_scaleY 1.0f;
d f m_scaleZ 1.0f;

c BodyInitData
d f m_positionX 0.0
d f m_positionY 0.0
d f m_positionZ 0.0
d f m_scaleX 1.0f;
d f m_scaleY 1.0f;
d f m_scaleZ 1.0f;
d i m_type 3
d i m_static 0
d i m_collisionEnabled 1
d i m_compound 0
d i m_impulseEnabled 1
d f m_mass 0.25

c PhysicsBody

c ShipModule
d f m_maxhealth 100.0f
d f m_maxvalue 100.0f

c ShieldModule
d f cooldownTime 2.0
d f activationTime 0.25
d f impulse 1000.0
d f maxRange 100.0
d f minRange 5.0