# Not yet implemented!
e LevelGenServer

# The transform specifies the first chamber's start transformation data.
c Transform
d f m_translationX 10.0
d f m_translationY 10.0
d f m_translationZ 10.0

c LevelInfo
# Specify if rotation of the first chamber should be randomised or not
# 1 for true, 0 for false.
d i m_randomStartRotation 0
# Branching count - Specifies how many times the level generator system should
# branch outwards.
d i m_branchCount 1
# The info will store all chambers and tunnel types, reusing the variables.
# These are stored as array data in the component when parsed.
# Piece 1
d i begin_piece 1
d s m_pieceAssemblageName rocksServer
d s m_pieceModelName WorldPrison_FinalWork_9_exporting.agl
d i m_pieceWeight 1
# Piece 2
d i begin_piece 2
d s m_pieceAssemblageName tunnelServer
d s m_pieceModelName World_Tunnel_3Way.agl
d i m_pieceWeight 0
# Piece N