e ClientShip

c LoadMesh
d s m_filename Ship.agl

c SpeedBuffer

c LightsComponent
# Left lantern
d s 0:typeAsStr point
d f 0:scaleX 20
d f 0:scaleY 20
d f 0:scaleZ 20
d f 0:translationX -5
d f 0:translationZ 5

d f 0:range 20
d f 0:attenuationZ 0.6
d f 0:diffuseR 0.8
d f 0:specularR 0.8

# Right lantern
d s 1:typeAsStr point
d f 1:scaleX 20.0
d f 1:scaleY 20.0
d f 1:scaleZ 20.0
d f 1:translationX 5.0
d f 1:translationZ 5.0

d f 1:range 20
d f 1:attenuationZ 0.6
d f 1:diffuseG 0.8
d f 1:specularG 0.8

# Main floodlight
d string 	2:typeAsStr 	"spot"
d float 	2:range 		500.0
d float 	2:spotPower 	30.0
d float 	2:lightEnergy 	3
d vec3 		2:translation 	0, 		0,		10
d vec3 		2:color 		0.98,	0.79,	0.3
d vec3 		2:scale 		100, 	500, 	100
d vec3 		2:lightDir 		0, 		1,		0

# Engine
d s 3:typeAsStr point
d f 3:scaleX 100
d f 3:scaleY 100
d f 3:scaleZ 100
d f 3:translationZ -6.5

d f 3:range 100
d f 3:attenuationX 0.0
d f 3:attenuationY 0.0
d f 3:attenuationZ 0.01
d f 3:diffuseR 0.3
d f 3:diffuseG 0.2
d f 3:diffuseB 0.1
d f 3:specular 0.2

c SkeletalAnimation

d f m_fps 24

d s m_name Default
d i m_startFrame 280
d i m_endFrame 340

c DamageComponent
