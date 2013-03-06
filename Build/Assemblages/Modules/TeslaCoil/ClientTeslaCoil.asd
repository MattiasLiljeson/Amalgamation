e ClientTeslaCoil

c Transform
d f m_translationX 0.0
d f m_translationY 0.0
d f m_translationZ 0.0

c LoadMesh
d s m_filename TeslaCoil.agl

c ShipModule

c LightsComponent
# crazy light
d string 	2:typeAsStr 	"point"
d float 	2:range 		15.0
d float 	2:lightEnergy 	1.0	
d vec3 		2:translation 	0, 		6,		0
d vec3 		2:color 		0.2,	0.6,	0.8
d vec3 		2:scale 		15, 	15, 	15
d vec3 		2:lightDir 		0, 		1,		0

c SkeletalAnimation

d f m_fps 24

d s m_name Default
d i m_startFrame 110
d i m_endFrame 159

d s m_name Start
d i m_startFrame 1
d i m_endFrame 71

d s m_name Active
d i m_startFrame 80
d i m_endFrame 91

d s m_name Death
d i m_startFrame 180
d i m_endFrame 220