# Work in progress!
e LevelGenServer

# The transform specifies the first chamber's start transformation data.
# Scaling is unsupported.
c Transform
	d f m_translationX 10.0
	d f m_translationY 10.0
	d f m_translationZ 10.0

c LevelInfo
	# Specify if rotation of the first chamber should be randomised or not
	# 1 for true, 0 for false.
	d i m_randomStartRotation 	0


	# Branching count - Specifies how many times the level generator system should
	# branch outwards. If the branching count is set to 0, only one chamber will be
	# created. If the branching count is set to -1, the player will branch as many
	# times needed until the desired size depending on the player count is achieved.
	d i m_branchCount 			-1

	# Level size - Specifies how large the level should be in size.
	# The size in the game is calculated by adding all chambers' radiuses together.
	d s m_levelSize 1:1500
	d s m_levelSize 2:1500
	d s m_levelSize 3:2000
	d s m_levelSize 4:3000
	d s m_levelSize 5:3500
	d s m_levelSize 6:4500
	d s m_levelSize 7:5000
	d s m_levelSize 8:6000

	# Level size precision specified how accurate the size should be. For instance,
	# if the desired size is 2000, and the level genererated becomes 2400, this is
	# ok, since it's within the precision range.
	# Not yet implemented!
	d i m_levelSizePrecision	300
	
	# The info will store all chambers and tunnel types, reusing the variables.
	# These are stored as array data in the component when parsed.
	# piece begin starts a new piece block section to be read.
	# piece end finishes a piece block section and saves it.

	# Piece 0
	d s piece begin
	d s m_pieceAssemblageFileName 	prisonChamberServer.asd
	d s m_pieceModelName 			WorldPrison_FinalWork_9_exporting.agl
	d i m_pieceWeight 				1
	d s piece end
	# Piece 1
	d s piece begin
	d s m_pieceAssemblageFileName 	vegetationChamberServer.asd
	d s m_pieceModelName 			vegetationchamber.agl
	d i m_pieceWeight 				1
	d s piece end
	# Piece 2
	d s piece begin
	d s m_pieceAssemblageFileName 	alienChamberServer.asd
	d s m_pieceModelName 			BlueAlienChamber2.agl
	d i m_pieceWeight 				1
	d s piece end
	# Piece 3
	d s piece begin
	d s m_pieceAssemblageFileName 	mineChamberServer.asd
	d s m_pieceModelName 			World_Mine.agl
	d i m_pieceWeight 				3
	d s piece end
	# Piece 4
	d s piece begin
	d s m_pieceAssemblageFileName 	ancientChamberServer.asd
	d s m_pieceModelName 			ancientchamber.agl
	d i m_pieceWeight				1
	d s piece end
	# Piece 5
	d s piece begin
	d s m_pieceAssemblageFileName 	sandChamberServer.asd
	d s m_pieceModelName			World_Sand.agl
	d i m_pieceWeight 				0
	d s piece end
	# Piece 6
	d s piece begin
	d s m_pieceAssemblageFileName 	smallchamber1Server.asd
	d s m_pieceModelName 			small_chamber1.agl
	d i m_pieceWeight 				3
	d s piece end
	# Piece 7
	d s piece begin
	d s m_pieceAssemblageFileName 	smallchamber2Server.asd
	d s m_pieceModelName 			small_chamber2.agl
	d i m_pieceWeight 				1
	d s piece end
	# Piece 8
	d s piece begin
	d s m_pieceAssemblageFileName 	smallchamber3Server.asd
	d s m_pieceModelName 			small_chamber3.agl
	d i m_pieceWeight 				3
	d s piece end
	# Piece 9
	d s piece begin
	d s m_pieceAssemblageFileName 	smallchamber4Server.asd
	d s m_pieceModelName 			small_chamber4.agl
	d i m_pieceWeight 				3
	d s piece end
	# Piece X



	# Starting piece of this generator [0 to N-1] where N is the amount of pieces 
	# above that will be loaded. Specify -1 to randomly select one.
	d i m_startPieceId 				3

	# End piece to plug connection points with.
	# The end plug piece must be specified after all other pieces.
	d s m_plugAssemblageOpened		plugPieceOpenedServer.asd
	d s m_plugModelOpened			Gate_open.agl
	d s m_plugAssemblageClosed		plugPieceClosedServer.asd
	d s m_plugModelClosed			Gate_closed.agl