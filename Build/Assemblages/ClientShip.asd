e ClientShip

c LoadMesh
d s m_filename Ship.agl

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
d s 2:typeAsStr spot
d f 2:scaleX 100
d f 2:scaleY 1000
d f 2:scaleZ 100
d f 2:translationZ 10

d f 2:range 1000.0
d f 2:spotPower 8.0
d f 2:attenuationX 0.1
d f 2:attenuationY 0.0
d f 2:attenuationZ 0.0
d f 2:diffuseR 0.3
d f 2:diffuseG 0.25
d f 2:diffuseB 0.2
d f 2:specular 0.3

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