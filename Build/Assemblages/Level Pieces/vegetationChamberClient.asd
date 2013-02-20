e vegetationChamberClient

c LoadMesh
d s m_filename vegetationchamber.agl

# Values for the atmosphere of a chamber
c EnvironmentValues
d f m_fogColorR 0.38
d f m_fogColorG 0.58
d f m_fogColorB 0.3764
d f m_ambientColorR 0.259
d f m_ambientColorG 0.169
d f m_ambientColorB 0.157
# The values specifying the closeness(in percentage range 0-1) 
# of the fog start and end in relation to the near- and far planes.
d f m_fogFarPlaneClosenessPercentage 1.0f
d f m_fogNearPlaneClosenessPercentage 1.0f
# Radius of the "fog-volume" covering the chamber
d f m_radius 500.0

# Transform data on the client is going to be overrided by the packet that is sent from the server.
c Transform
d f m_translationX 0.0
d f m_translationY 0.0
d f m_translationZ 0.0

c LevelPieceRoot