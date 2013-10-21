#define FORCE_VS_DBG_OUTPUT

#include "LevelGenSystem.h"
#include "Transform.h"
#include "RenderInfo.h"
#include <AglVector3.h>
#include <sstream>
#include <DebugUtil.h>
#include "LevelPiece.h"
#include <cstdlib>
#include <ctime>
#include <AglCollision.h>
#include "StaticProp.h"
#include <TcpServer.h>
#include "EntityCreationPacket.h"
#include "EntityType.h"
#include <Globals.h>
#include <ModelResource.h>
#include <string>
#include "GameplayTags.h"
#include "BodyInitData.h"
#include "PhysicsBody.h"
#include "BoundingVolumeInitData.h"
#include "LoadMeshSystemServer.h"
#include "EntityFactory.h"
#include "LevelInfo.h"
#include "LevelPieceRoot.h"
#include <OutputLogger.h>
#include <numeric>
#include <array>

LevelGenSystem::LevelGenSystem(TcpServer* p_server) 
	: EntitySystem(SystemType::LevelGenSystem, 1, ComponentType::LevelInfo)
{
	m_server = p_server;

	m_worldMin = AglVector3((float)INT_MAX, (float)INT_MAX, (float)INT_MAX);
	m_worldMax = AglVector3((float)INT_MIN, (float)INT_MIN, (float)INT_MIN);

	m_entityFactory			= NULL;
	m_levelInfo				= NULL;
	m_endPlugModelResource	= NULL;
	m_readyToRun			= false;
	m_hasGeneratedLevel		= false;
	m_useModifiedVersion	= false;
}

LevelGenSystem::~LevelGenSystem()
{
	clearGeneratedData();
	// Clear model resources (Don't delete, since the level gen doesn't have ownership)
	m_modelResources.clear();
}

void LevelGenSystem::initialize()
{
	m_entityFactory = static_cast<EntityFactory*>(
		m_world->getSystem(SystemType::EntityFactory));

	preloadLevelGenRecipeEntity( LEVELPIECESPATH + "LevelGenServer.asd");
}

void LevelGenSystem::calculatePieceCollision( vector<ModelResource*>* p_pieceMesh )
{
	// Index 0 always contains the root.
	if (p_pieceMesh->size() > 0)
	{
		AglBoundingSphere boundingSphere = p_pieceMesh->at(1)->meshHeader.boundingSphere;
		AglMatrix mat ;

		for (unsigned int i = 2; i < p_pieceMesh->size(); i++)
		{
			ModelResource* resource = p_pieceMesh->at(i);
			AglBoundingSphere nextBoundingSphere = resource->meshHeader.boundingSphere;
			
			AglVector3 pos = (resource->meshHeader.transform * resource->transform).GetTranslation();
			nextBoundingSphere.position = pos;

			nextBoundingSphere.radius = abs(nextBoundingSphere.radius);
			boundingSphere = AglBoundingSphere::mergeSpheres(boundingSphere, 
				nextBoundingSphere);
		}
		string logtext = p_pieceMesh->at(0)->name + " : r=" + toString(boundingSphere.radius) + "\n";
		m_world->getOutputLogger()->write( logtext.c_str() );

		p_pieceMesh->at(0)->meshHeader.boundingSphere = boundingSphere;
	}
}

void LevelGenSystem::preloadLevelGenRecipeEntity(const string& p_filePath)
{
	string recipeName;
	m_entityFactory->readAssemblageFile(p_filePath, &recipeName);
	Entity* e = m_entityFactory->entityFromRecipe(recipeName);
	m_world->addEntity(e);
}

void LevelGenSystem::inserted( Entity* p_entity )
{
	m_levelInfo			= static_cast<LevelInfo*>(p_entity->getComponent(ComponentType::LevelInfo));
	m_startTransform	= static_cast<Transform*>(p_entity->getComponent(ComponentType::Transform))->getMatrix();

	auto loadMeshSys = static_cast<LoadMeshSystemServer*>(
		m_world->getSystem(SystemType::LoadMeshSystem));

	vector<LevelPieceFileData*> fileData = m_levelInfo->getFileData();
	for (int i = 0; i < fileData.size(); i++)
	{
		string modelName = fileData[i]->modelFileName;	

		// Preload chamber models here. This is required, and must be done before an
		// entity is created.
		auto resourcesFromModel = loadMeshSys->createModels(modelName,
			MODELPATH, false);

		ModelResource* rootResource = resourcesFromModel->at(0);

		// Preload the chamber assemblages.
		m_entityFactory->readAssemblageFile(LEVELPIECESPATH + fileData[i]->assemblageFileName,
											&fileData[i]->assemblageName);

		// Calculate the entire chamber's collision sphere.
		// NOTE: Uncertain whether or not this value should be multiplied by 2 or not
		// before being used.
		calculatePieceCollision(resourcesFromModel);

		// Add model resource to collection. This also checks the bounding sphere and
		// inserts the resource index to another vector with a sorted order of its bounding sphere.
		addResource(rootResource);
		
		//delete resourcesFromModel;
	}
	for (int i = 0; i < m_sortedResourceIds.size(); i++)
	{
		float radius = m_modelResources[m_sortedResourceIds[i]]->meshHeader.boundingSphere.radius;
		string logtext = " : r=" + toString(radius) + "\n";
		m_world->getOutputLogger()
			->write(logtext.c_str());
	}

	LevelPieceFileData* endPlug = m_levelInfo->getEndPlugFileData(ENDPIECEMODE_CLOSED);
	m_entityFactory->readAssemblageFile(LEVELPIECESPATH + endPlug->assemblageFileName,
		&endPlug->assemblageName);
	auto resourcesFromModel = loadMeshSys->createModels(endPlug->modelFileName,
		MODELPATH, false);
	m_endPlugModelResource = resourcesFromModel->at(0);
	
	endPlug = m_levelInfo->getEndPlugFileData(ENDPIECEMODE_OPENED);
	m_entityFactory->readAssemblageFile(LEVELPIECESPATH + endPlug->assemblageFileName,
		&endPlug->assemblageName);
	resourcesFromModel = loadMeshSys->createModels(endPlug->modelFileName,
		MODELPATH, false);
	m_endPlugOpenedModelResource = resourcesFromModel->at(0);


	m_readyToRun = true;
	srand(static_cast<unsigned int>(time(NULL)));

	std::cout << "Done with init\n";

	//* ALEX experiment, run the level gen 20 times! *//
	//std::vector<int> diameters;

	const int amountOfRuns = 10000;
	const int intervalSize = 18;
	const int intervalSpread = 1000;

	// Data collected for normal algorithm
	std::array<int, intervalSize> categorySortedDataA;
	categorySortedDataA.fill(0);

	int minA = INT_MAX;
	int maxA = INT_MIN;
	int avgA = 0;
	int sumA = 0;

	// Data collected for modified algorithm
	std::array<int, intervalSize> categorySortedDataB;
	categorySortedDataB.fill(0);

	int minB = INT_MAX;
	int maxB = INT_MIN;
	int avgB = 0;
	int sumB = 0;

	std::ofstream outfile("levelgen_result_diameter_cluster.txt", std::ifstream::out | std::ifstream::app);
	if (outfile.is_open())
	{
		outfile << "# Sample Normal Modified\n";
		// Run experiment, collect diameter data, and find min/max of data
		for (int i = 0; i < amountOfRuns; i++)
		{
			//outfile = std::ofstream("levelgen_out_spheres.txt", std::ifstream::out | std::ifstream::app);
			//if (outfile.is_open())
			//{
			//	outfile << "###\n";
			//	outfile.close();
			//}
			m_hasGeneratedLevel = false;
			m_useModifiedVersion = false;
			generateLevel(8);
			categorySortedDataA[m_levelTreeDiameter/intervalSpread]++;
		
			if (minA > m_levelTreeDiameter)
				minA = m_levelTreeDiameter;
			if (maxA < m_levelTreeDiameter)
				maxA = m_levelTreeDiameter;
			sumA += m_levelTreeDiameter;
			
			outfile << (i+1) << " " << m_levelTreeDiameter;

			//outfile = std::ofstream("levelgen_out_spheres.txt", std::ifstream::out | std::ifstream::app);
			//if (outfile.is_open())
			//{
			//	outfile << "size " << m_currentLevelSize << "\ndiameter " << m_levelTreeDiameter << "\n";
			//	outfile.close();
			//}
			/*outfile = std::ofstream("levelgen_result_size_diameter.txt",  std::ifstream::out | std::ifstream::app);
			if (outfile.is_open())
			{
				outfile << (i+1) << " " << m_levelTreeDiameter << "\n";
				//outfile << (i+1) << " " << m_currentLevelSize << " " << m_levelTreeDiameter << "\n";
				outfile.close();
			}*/

			m_hasGeneratedLevel = false;
			m_useModifiedVersion = true;
			generateLevel(8);
			categorySortedDataB[m_levelTreeDiameter/intervalSpread]++;

			if (minB > m_levelTreeDiameter)
				minB = m_levelTreeDiameter;
			if (maxB < m_levelTreeDiameter)
				maxB = m_levelTreeDiameter;
			sumB += m_levelTreeDiameter;

			outfile << " " << m_levelTreeDiameter << "\n";

			//diameters.push_back(m_levelTreeDiameter);
		}
		avgA = static_cast<int>(1.0 * sumA / amountOfRuns);
		avgB = static_cast<int>(1.0 * sumB / amountOfRuns);

		outfile << "# avg " << avgA << " " << avgB << "\n";
		outfile << "# min " << minA << " " << minB << "\n";
		outfile << "# max " << maxA << " " << maxB << "\n";

		outfile.close();

		std::cout << "Done with generating and collecting data, and min/max/avg\n";
		std::cout << "Done with writing diameter data to file [pass 1]\n";
	}
	//sort(diameters.begin(), diameters.end());
	outfile = std::ofstream("levelgen_result_diameter_interval.txt", std::ifstream::out | std::ifstream::app);
	if (outfile.is_open())
	{
		outfile << "# DiameterInterval Normal Modified\n";

		for (int i = 0; i < intervalSize; i++)
		{
			string interval = "\"" + toString(i*intervalSpread) + "-" + toString((i+1)*intervalSpread-1) + "\"";
			outfile << interval << " " << categorySortedDataA[i] << " " << categorySortedDataB[i] << "\n";
			//outfile << (i+1) << " " << m_currentLevelSize << " " << m_levelTreeDiameter << "\n";
		}
		outfile.close();
	}
	std::cout << "Done with writing diameter data to file [pass 2]\n";

	std::cout << "Done with experiment\n";
	//m_world->deleteEntity(p_entity);
}

void LevelGenSystem::removed( Entity* p_entity )
{
	m_levelInfo = NULL;
}

void LevelGenSystem::clearGeneratedData()
{
	// Destroy generated pieces.
	for (unsigned int i = 0; i < m_generatedPieces.size(); i++)
	{
		delete m_generatedPieces[i];
	}
	m_generatedPieces.clear();
	m_endPlugs.clear();
	m_pieceIds.clear();
	m_levelTreeDiameter = 0;
	// There's still data that exists, such as init data. These should not be destroyed
	// or cleared here.
}

void LevelGenSystem::generateLevel(int p_nrOfPlayers)
{
	if (!m_hasGeneratedLevel)
	{
		if (m_readyToRun)
		{
			clearGeneratedData();

			m_nrOfPlayers = p_nrOfPlayers;
			generateLevelPieces(m_levelInfo->getBranchCount(), m_levelInfo->doRandomStartRotation());
			createLevelEntities();
			m_hasGeneratedLevel = true;
		}
		else
		{
			m_world->getOutputLogger()
				->write("LevelGenSystem is not ready to generate level yet.\n", WRITETYPE_WARNING);
		}
	}
	else
	{
		m_world->getOutputLogger()
			->write("LevelGenSystem has already generated a level.\n", WRITETYPE_WARNING);
	}
}

void LevelGenSystem::generateLevelPieces( int p_maxDepth, bool p_doRandomStartRotation)
{
	// Creates the first entity.
	AglQuaternion quart;
	if (p_doRandomStartRotation)
		quart = AglQuaternion::constructFromAxisAndAngle(AglVector3::forward(),
														(rand() % 360) * 3.1415f / 180.0f);
	else
		quart = AglQuaternion::identity();

	// Create a initial piece.
	Transform* transform = new Transform(m_startTransform);
	transform->setRotation(quart);

	// Create the level piece to use later. Note, this could be random, if the specified
	// start piece id in the assemblage has been set to -1.
	int id = m_levelInfo->getStartFileData()->id;

	LevelPiece* piece = new LevelPiece(id, m_modelResources[id], transform, 0);
	piece->setPieceId(m_pieceIds.add(piece));

	// The first time, this vector will only contain the initial piece.
	vector<LevelPiece*> pieces;
	pieces.push_back(piece);
	m_generatedPieces.push_back(piece);
	printAddedLevelPiece(piece);

	int usedMaxDepth;

	if (p_maxDepth == -1)
	{
		m_useLevelMaxSize	= true;
		usedMaxDepth		= 10000;
		m_levelMaxSize		= m_levelInfo->getLevelSize(m_nrOfPlayers);
	}
	else
	{
		m_useLevelMaxSize	= false;
		usedMaxDepth		= p_maxDepth;
	}
	m_hasHitLevelMaxSize = false;
	m_currentLevelSize = m_modelResources[id]->meshHeader.boundingSphere.radius;

	testLevelMaxSizeHit();

	// The algorithm generates pieces outwards based on a so called depth. For each depth
	// the pieces from the previous depth are used to create and connect new pieces.
	// If the specified depth is set to -1, then the level gen will generate a level that
	// depends on the number of players in game.
	for (int currentDepth = 0; 
		currentDepth < usedMaxDepth && !m_hasHitLevelMaxSize && !pieces.empty(); 
		currentDepth++)
	{
		// Stores created pieces temporarily
		vector<LevelPiece*> temps;
		for (unsigned int i = 0; i < pieces.size(); i++)
			generatePiecesOnPiece(pieces[i], temps, currentDepth+1);

		pieces = vector<LevelPiece*>(temps);
	}

	testLevelMaxSizeHit();

	int maxDiameter = computeDiameterOfTree(piece);
	m_levelTreeDiameter = maxDiameter;
	m_world->getOutputLogger()
		->write(("The created level has a diameter of " + toString(maxDiameter) + "\n").c_str(), WRITETYPE_INFO);
}

Entity* LevelGenSystem::createEntity( LevelPiece* p_piece)
{
	Entity* entity = m_entityFactory->entityFromRecipe( 
		m_levelInfo->getFileDataFromId( p_piece->getTypeId() )->assemblageName );
	
	if (!entity)
	{
		m_world->getOutputLogger()
			->write("LevelGenSystem was unable to create the specified level piece entity!\n", WRITETYPE_WARNING);
	}
	else
	{
		auto transform = static_cast<Transform*>(
			entity->getComponent(ComponentType::Transform));

		if (!transform)
		{
			transform = new Transform();
			entity->addComponent( transform );
			transform->setTranslation( p_piece->getTransform()->getTranslation() );
			transform->setRotation( p_piece->getTransform()->getRotation() );
		}
		// ISSUE?!? Scale can not be transferred from the piece.
		transform->setTranslation( p_piece->getTransform()->getTranslation() );
		transform->setRotation( p_piece->getTransform()->getRotation() );

		entity->addComponent(new StaticProp(p_piece->getTypeId(), STATICPROPTYPE_LEVELPIECE));

		auto pieceRoot = static_cast<LevelPieceRoot*>(
			entity->getComponent(ComponentType::LevelPieceRoot));

		pieceRoot->pieceId = p_piece->getPieceId();
		pieceRoot->boundingSphere = p_piece->getBoundingSphere();

		pieceRoot->pieceRootType = PIECEROOTTYE_CHAMBER;

		// === ALEX logging stuff ===
		auto spherePos = p_piece->getBoundingSphere().position;//piece->getTransform()->getTranslation();
		float radius = p_piece->getBoundingSphere().radius;
		/*std::ofstream matlabOut("matlab_out_pieces.m", std::ifstream::out | std::ifstream::app);
		if (matlabOut.is_open())
		{
			matlabOut << "surf(" << pos.x << "+x*" << radius << ", " << pos.y << "+y*" << radius << ", " << pos.z << "+z*" << radius << ");\n";
			matlabOut.close();
		}*/
		std::ofstream outfile("levelgen_out_spheres.txt", std::ifstream::out | std::ifstream::app);
		if (outfile.is_open())
		{
			outfile << spherePos.x << " " << spherePos.y << " " << spherePos.z << " " << (radius*2) << " " << p_piece->getTypeId() << "\n";
			outfile.close();
		}

		pieceRoot->connectedRootPieces.resize(p_piece->getMaxChildCount(), -1);
		for (unsigned int i = 0; i < p_piece->getMaxChildCount(); i++)
		{
			pieceRoot->connectedRootPieces[i] = p_piece->getGate(i);
		}
		int i = 0;
	}

	return entity;
}

Entity* LevelGenSystem::createDebugSphereEntity( LevelPiece* p_piece )
{
	/* DEBUG */
	//AglBoundingSphere boundingSphere = p_piece->getBoundingSphere();
	//AglVector3 scale(boundingSphere.radius, boundingSphere.radius, boundingSphere.radius);
	////scale *= 2;

	//Entity* e = m_world->createEntity();
	//e->addComponent(new Transform(boundingSphere.position, AglQuaternion::identity(),
	//	scale));
	//e->addComponent(new StaticProp(1));
	//m_world->addEntity(e);
	//return e;
	/* END OF DEBUG */
	return NULL;
}

void LevelGenSystem::generatePiecesOnPiece( LevelPiece* p_targetPiece, 
										   vector<LevelPiece*>& out_pieces, int p_generation )
{
	vector<int> freeConnectSlots = p_targetPiece->findFreeConnectionPointSlots();

	// Check that there's any available pipes
	if ( !freeConnectSlots.empty() )
	{
		// Prepare and connect newly created transform pieces to the target.
		int maxPiecesCount = freeConnectSlots.size();
		for (int i = 0; i < maxPiecesCount && !m_hasHitLevelMaxSize; i++)
		{
			// TODO: ??? Currently, if the randomized piece type collides with other pieces
			// on the desired position, it will set that corresponding slot on the target
			// to be occupied. For better generating, a new piece type should be selected
			// and collision tested until there's no valid piece type at all, before
			// setting the target connector to be occupied.

			// Find a random piece type to use
			int pieceType = m_levelInfo->getRandomFileData()->id;
			
			int slot = popIntVector(freeConnectSlots);

			// Create a level piece
			LevelPiece* piece = nullptr;

			bool success;
			bool outOfOptions = false;

			do 
			{
				piece = new LevelPiece( pieceType, m_modelResources[pieceType],
					new Transform(), p_generation);
				
				success = tryConnectPieces(p_targetPiece, piece, slot);

				// If it was a failure, a new test needs to be initiated
				// A new random piece will be selected as long as there is one to select, that is smaller than the previous 
				if (!success)
				{
					if (m_useModifiedVersion)
					{
						int upperBound = 0;
						for (int i = 0; i < m_sortedResourceIds.size(); i++)
							if (m_sortedResourceIds[i] == pieceType)
								upperBound = i;
	
						if (upperBound > 0)
						{
							string info = "The piece type " + toString(pieceType) + " with radius " + toString(piece->getBoundingSphere().radius) + " is too large\n";
							m_world->getOutputLogger()
								->write(info.c_str());
	
							pieceType = m_sortedResourceIds[rand() % upperBound];
							
							info = "Attempting to select a new piece type: " + toString(pieceType) + "\n";
							m_world->getOutputLogger()
								->write(info.c_str());
						}
						else
							outOfOptions = true;
					}
					else
						outOfOptions = true;

					delete piece;
					piece = nullptr;
				}
			} while (!success && !outOfOptions);

			if (outOfOptions)
			{
				string info = "Out of piece options\n";
				m_world->getOutputLogger()
					->write(info.c_str());
			}

			if (success)//tryConnectPieces(p_targetPiece, piece, slot))
			{
				string info = "Successfully connected piece type: " + toString(pieceType) + "\n";
				m_world->getOutputLogger()
					->write(info.c_str());

				// Add an open gate
				Entity* ent = addEndPlug(&p_targetPiece->getConnectionPoint(slot), ENDPIECEMODE_OPENED);
				int gateUid = m_pieceIds.add(ent);
				p_targetPiece->setGate(slot, gateUid);
				piece->setGate(0, gateUid);

				info = "Added open gate with id " + toString(gateUid) + "\n";
				m_world->getOutputLogger()
					->write(info.c_str());

				m_world->addEntity(ent);

				// This is not done from here anymore.
				//AglOBB obb = piece->getBoundingBox();
				//updateWorldMinMax(obb);
				if (m_useLevelMaxSize)
				{
					m_currentLevelSize += 
						m_modelResources[pieceType]->meshHeader.boundingSphere.radius;

					testLevelMaxSizeHit();
				}

				out_pieces.push_back(piece);

				int newPieceUid = m_pieceIds.add(piece);
				piece->setPieceId(newPieceUid);
				printAddedLevelPiece(piece);

				// Connect the gate with the two chambers
				auto plugPieceRoot = static_cast<LevelPieceRoot*>(ent->getComponent(ComponentType::LevelPieceRoot));
				plugPieceRoot->pieceId = gateUid;
				plugPieceRoot->pieceRootType = PIECEROOTTYE_GATE;
				plugPieceRoot->connectedRootPieces.resize(2, -1);
				plugPieceRoot->connectedRootPieces[CHAMBERSIDE_PARENT]	= p_targetPiece->getPieceId();
				plugPieceRoot->connectedRootPieces[CHAMBERSIDE_CHILD]	= piece->getPieceId();

				m_generatedPieces.push_back(piece);
			}
		}
	}

}

int LevelGenSystem::popIntVector( vector<int>& p_vector )
{
	int i = -1;
	if ( !p_vector.empty() )
	{
		i = p_vector.back();
		p_vector.pop_back();
	}
	return i;
}

void LevelGenSystem::createLevelEntities()
{
	for (unsigned int i = 0; i < m_generatedPieces.size(); i++)
	{
		addEndPlugs(m_generatedPieces[i]); // Endplugs needs to be generated before chamber entities.
		Entity* e = createEntity(m_generatedPieces[i]);
		m_world->addEntity(e);
		
		//e = createDebugSphereEntity(m_generatedPieces[i]);
		//m_world->addEntity(e);
	}
	m_world->getOutputLogger()
		->write((toString(m_generatedPieces.size()) + " chambers generated.\n").c_str());
}

const AglVector3&  LevelGenSystem::getWorldMin() const
{
	return m_worldMin;
}

const AglVector3&  LevelGenSystem::getWorldMax() const
{
	return m_worldMax;
}
vector<LevelPiece*> LevelGenSystem::getGeneratedPieces() const
{
	return m_generatedPieces;
}

void LevelGenSystem::updateWorldMinMax( AglOBB& boundingVolume )
{
	auto corners = boundingVolume.getCorners();

	for (unsigned int i = 0; i < corners.size(); i++)
	{
		m_worldMin = AglVector3::minOf(m_worldMin, corners[i]);
		m_worldMax = AglVector3::maxOf(m_worldMax, corners[i]);
	}
}

Entity* LevelGenSystem::addEndPlug( Transform* p_atConnector, EndPieceMode p_mode )
{
	Entity* entity = m_entityFactory->entityFromRecipe(
		m_levelInfo->getEndPlugFileData(p_mode)->assemblageName );

	auto transform = static_cast<Transform*>(
		entity->getComponent(ComponentType::Transform));

	AglVector3 tempScale = AglVector3::one();
	if (!transform)
	{
		transform = new Transform();
		entity->addComponent( transform );
	}
	else
	{
		tempScale = transform->getScale();
	}

	// Set proper rotation and scale of the plug piece
	transform->setMatrix(p_atConnector->getMatrix());
	transform->setForwardDirection( -transform->getForward() );
	// NOTE: Due to some circumstance, the gate needs to be rotated here. It wasn't needed
	// before, but now it does. // Alex
	transform->setRotation(transform->getRotation() 
		* AglQuaternion::constructFromAxisAndAngle(AglVector3::right(), 90 * 3.1415f / 180.0f));
	transform->setScale(tempScale);

	entity->addComponent(new StaticProp(m_levelInfo->getEndPlugFileData(p_mode)->id, STATICPROPTYPE_PLUGPIECE));

	// Manipulate the BodyInitData translation info to match the transform, and add a proper
	// model resource.
	auto bodyInitData = static_cast<BodyInitData*>(entity->getComponent(ComponentType::BodyInitData));
	if (bodyInitData)
	{
		bodyInitData->m_position		= transform->getTranslation();
		bodyInitData->m_orientation		= transform->getRotation();
		bodyInitData->m_scale			= transform->getScale();
		if (p_mode == ENDPIECEMODE_CLOSED)
			bodyInitData->m_modelResource = m_endPlugModelResource;
		else
			bodyInitData->m_modelResource = m_endPlugOpenedModelResource;
	}
	m_endPlugs.push_back(entity);
	return entity;
}

void LevelGenSystem::addEndPlugs(LevelPiece* p_atPiece)
{
	for (unsigned int i = 0; i < p_atPiece->getMaxChildCount(); i++)
	{
		if (!p_atPiece->isChildSlotOccupied(i))
		{
			Entity* plug = addEndPlug(&p_atPiece->getConnectionPoint(i), ENDPIECEMODE_CLOSED);
			int plugId = m_pieceIds.add(plug);
			p_atPiece->setGate(i, plugId);
			m_world->addEntity(plug);
			
			auto plugPieceRoot = static_cast<LevelPieceRoot*>(plug->getComponent(ComponentType::LevelPieceRoot));
			plugPieceRoot->pieceId = plugId;
			plugPieceRoot->pieceRootType = PIECEROOTTYE_GATE;
			plugPieceRoot->connectedRootPieces.resize(2, -1);
			plugPieceRoot->connectedRootPieces[CHAMBERSIDE_PARENT] = p_atPiece->getPieceId();

			string info = "Added end plug with id " + toString(plugId) + "\n";
			m_world->getOutputLogger()
				->write(info.c_str());
		}
	}
}

int LevelGenSystem::getGeneratedPiecesCount() const
{
	return m_generatedPieces.size();
}

bool LevelGenSystem::isLevelGenerated() const
{
	return m_hasGeneratedLevel;
}


void LevelGenSystem::resetState()
{
	m_readyToRun		= true;
	m_hasGeneratedLevel = false;
}

void LevelGenSystem::addResource(ModelResource* p_modelResource)
{
	AglBoundingSphere boundingSphere = p_modelResource->meshHeader.boundingSphere;

	// Add resource to collection
	m_modelResources.push_back(p_modelResource);

	int insertAtIndex = m_sortedResourceIds.size();
	// Check all previous bounding spheres.
	for (unsigned int i = 0; i < m_sortedResourceIds.size(); i++)
	{
		int resourceId = m_sortedResourceIds[i];
		if (m_modelResources[resourceId]->meshHeader.boundingSphere.radius > boundingSphere.radius)
		{
			insertAtIndex = i;
			break; // Break!
		}
	}
	m_sortedResourceIds.insert(m_sortedResourceIds.begin() + insertAtIndex, m_modelResources.size()-1);
}

bool LevelGenSystem::tryConnectPieces( LevelPiece* p_target, LevelPiece* p_newPiece, int p_slot )
{
	bool tooLarge = false;
	bool colliding = false;

	int newPieceRadius = (int)m_modelResources[p_newPiece->getTypeId()]->meshHeader.boundingSphere.radius;
	if (m_useLevelMaxSize && m_currentLevelSize + newPieceRadius > m_levelMaxSize)
	{
		m_world->getOutputLogger()
			->write("Chamber piece too large.\n");// A level plug has been created instead.\n");

		tooLarge = true;
	}
	p_newPiece->connectTo(p_target, p_slot);

	// Verify that it isn't colliding with previous level pieces.
	for (unsigned int i = 0; i < m_generatedPieces.size(); i++)
	{
		if (AglCollision::isColliding( p_newPiece->getBoundingSphere(),
			m_generatedPieces[i]->getBoundingSphere()) && 
			p_newPiece->getConnectedPiece(0) != m_generatedPieces[i] )
		{
			m_world->getOutputLogger()
				->write("Collision between chambers detected.\n");//A level plug has been created instead.\n");
			colliding = true;
			break;
		}
	}

	if (colliding || tooLarge)
	{
		// Remove the connected component.
		p_target->setConnectedPiece(p_slot, NULL);
		//delete p_newPiece;
		return false;
	}
	else
	{
		p_target->addChild(p_newPiece);
		return true;
	}
}

bool LevelGenSystem::testLevelMaxSizeHit()
{
	string logtext = "Current level size: " + toString(m_currentLevelSize) + "\n";
	m_world->getOutputLogger()
		->write( logtext.c_str() );

	if (m_useLevelMaxSize && m_currentLevelSize >= m_levelMaxSize)
	{
		logtext = "Max level size hit:\n\tdesired=" 
			+ toString(m_levelMaxSize) 
			+ "\n\tresulted="
			+ toString(m_currentLevelSize)
			+ "\n";

		m_world->getOutputLogger()
			->write( logtext.c_str() );
		m_hasHitLevelMaxSize = true;
	}
	return m_hasHitLevelMaxSize;
}

void LevelGenSystem::printAddedLevelPiece( LevelPiece* p_piece )
{
	string info = "Added level piece with id " + toString(p_piece->getPieceId()) + "\n";
	m_world->getOutputLogger()
		->write(info.c_str());
}

int LevelGenSystem::getLevelPieceRootCount()
{
	return m_pieceIds.getSize();
}

int LevelGenSystem::computeHeightOfTree( LevelPiece* p_node )
{
	if (p_node == nullptr)
		return 0;

	vector<int> heights;
	for each (auto child in p_node->getChildren())
	{
		heights.push_back(computeHeightOfTree(child));
	}
	int maxHeight = 0;
	if (!heights.empty())
		maxHeight = *max_element(heights.begin(), heights.end());

	// Return the height as the max height returned from the children,
	// + the radius of this node + the parent radius
	/*return p_node->getBoundingSphere().radius 
		+ p_node->getParent()->getBoundingSphere().radius 
		+ maxHeight;*/

	// A more accurate result is to take the distance between the child and the parent as height.
	/*AglVector3 deltaPos = p_node->getBoundingSphere().position - p_node->getParent()->getBoundingSphere().position;
	return maxHeight + AglVector3::length(deltaPos);*/

	AglVector3 deltaPos = p_node->getTransform()->getTranslation() - p_node->getParent()->getTransform()->getTranslation();
	return maxHeight + AglVector3::length(deltaPos);
}

int LevelGenSystem::computeDiameterOfTree( LevelPiece* p_node )
{
	if (p_node == nullptr)
		return 0;

	// Get the height of all sub-trees
	vector<int> heights;
	for each (auto child in p_node->getChildren())
	{
		heights.push_back(computeHeightOfTree(child));
	}
	// Get the diameter of all sub-trees
	vector<int> diameter;
	for each (auto child in p_node->getChildren())
	{
		diameter.push_back(computeDiameterOfTree(child));
	}

	/* Return max of the following:
	*	Diameter of any of the subtrees
	*	Height of all subtrees + the current node's radius + parent node's radius
	*/
	sort(heights.begin(), heights.end());
	int diameterThroughSelf = 0;
	if (heights.size() >= 1)
	{
		diameterThroughSelf = heights.back();
		if (heights.size() >= 2)
			diameterThroughSelf += heights[heights.size()-2];
	}
		
	int maxChildDiameter = 0;
	if (!diameter.empty())
		maxChildDiameter = *max_element(diameter.begin(), diameter.end());

	return max(diameterThroughSelf, maxChildDiameter);
}













