e ClientShip

c LightsComponent

# Left lantern
d s 0:typeAsStr point
d f 0:scaleX 10
d f 0:scaleY 10
d f 0:scaleZ 10
d f 0:translationZ -10
d f 0:translationY 10

d f 0:range 100
d f 0:attenuationX 1000.0
d f 0:attenuationY 1000.0
d f 0:attenuationZ 1000.0
d f 0:diffuseR 0.8
d f 0:specularR 0.8

# Right lantern
d s 1:typeAsStr point
d f 1:scaleX 10
d f 1:scaleY 10
d f 1:scaleZ 10
d f 1:translationX -10.0
#d f 1:translationY 10

d f 1:range 1000
d f 1:attenuation 1000.0
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
d f 2:attenuationX 1000.0
d f 2:attenuationY 1000.0
d f 2:attenuationZ 1000.0
d f 2:diffuse 0.3
d f 2:specular 0.3

# Engine 
d s 3:typeAsStr point
d f 3:scaleX 10
d f 3:scaleY 10
d f 3:scaleZ 10
d f 3:translationX -16.5

d f 3:range 100
d f 3:attenuationX 1000.0
d f 3:attenuationY 1000.0
d f 3:attenuationZ 1000.0
d f 3:diffuseG 0.8
d f 3:specularG 0.8