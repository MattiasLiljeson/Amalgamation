e Rocket

c Transform
	d f m_translationX 0.0
	d f m_translationY 0.0
	d f m_translationZ 0.0

c LoadMesh
	d s m_filename rocket.agl

c SkeletalAnimation
	d f m_fps 24
	d s m_name Default
	d i m_startFrame 10
	d i m_endFrame 40

# Rocket flame light
c LightsComponent
	d string 	2:typeAsStr 	"point"
	d float 	2:range 		100.0
	d float 	2:spotPower 	30.0
	d float 	2:lightEnergy 	3
	d vec3 		2:translation 	0, 		0,		0
	d vec3 		2:color 		0.8,	0.8,	0.5
	d vec3 		2:scale 		100, 	100, 	100
	d vec3 		2:lightDir 		0, 		1,		0