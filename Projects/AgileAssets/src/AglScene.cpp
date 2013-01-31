#include "AglScene.h"

AglScene::AglScene()
{
}
AglScene::~AglScene()
{
	for (unsigned int i = 0; i < m_meshes.size(); i++)
		delete m_meshes[i];
	for (unsigned int i = 0; i < m_animationLayers.size(); i++)
		delete m_animationLayers[i];
	for (unsigned int i = 0; i < m_nodeAnimations.size(); i++)
		delete m_nodeAnimations[i];
	for (unsigned int i = 0; i < m_animations.size(); i++)
		delete m_animations[i];
	for (unsigned int i = 0; i < m_skeletons.size(); i++)
		delete m_skeletons[i];
	for (unsigned int i = 0; i < m_materials.size(); i++)
		delete m_materials[i];
	for (unsigned int i = 0; i < m_skeletonMappings.size(); i++)
		delete m_skeletonMappings[i];
	for (unsigned int i = 0; i < m_bspTrees.size(); i++)
		delete m_bspTrees[i];
	for (unsigned int i = 0; i < m_sphereGrids.size(); i++)
		delete m_sphereGrids[i];
	for (unsigned int i = 0; i < m_particleSystems.size(); i++)
		delete m_particleSystems[i];
}
void AglScene::init(AglSceneDesc p_desc)
{
	m_meshes = p_desc.meshes;
	m_particleSystems = p_desc.particleSystems;
	m_nodes = p_desc.nodes;
	for (unsigned int i = 0; i < m_nodes.size(); i++)
	{
		AglDynamicNode n;
		n.animated = false;
		m_dynamicNodes.push_back(n);
	}
	m_animationLayers = p_desc.animationLayers;
	m_nodeAnimations = p_desc.nodeAnimations;
	m_animations = p_desc.animations;
	m_skeletons = p_desc.skeletons;
	m_materials = p_desc.materials;
	m_names		= p_desc.names;
	m_materialMappings = p_desc.materialMappings;
	m_skeletonMappings = p_desc.skeletonMappings;
	m_bspTrees = p_desc.bspTrees;
	m_sphereGrids = p_desc.sphereGrids;
	m_connectionPoints = p_desc.connectionPoints;
	m_particleSystems = p_desc.particleSystems;
	m_currentAnimation = 0;
	m_coordinateSystem = p_desc.coordinateSystem;
}

AglNode AglScene::getNode(int p_index)
{
	return m_nodes[p_index];
}

AglAnimationLayer* AglScene::getAnimationLayer(int p_index)
{
	return m_animationLayers[p_index];
}
AglNodeAnimation* AglScene::getNodeAnimation(int p_index)
{
	return m_nodeAnimations[p_index];
}
AglDynamicNode AglScene::getDynamicNode(int p_index)
{
	return m_dynamicNodes[p_index];
}
AglSkeleton* AglScene::getSkeleton(int p_index)
{
	return m_skeletons[p_index];
}
AglAnimation* AglScene::getAnimation(int p_index)
{
	return m_animations[p_index];
}
unsigned int AglScene::getSkeletonCount()
{
	return m_skeletons.size();
}

void AglScene::appendTransform(int p_index, AglMatrix p_transform)
{
	if (m_dynamicNodes[p_index].animated)
	{
		m_dynamicNodes[p_index].transform += p_transform;
	}
	else
	{
		m_dynamicNodes[p_index].animated = true;
		m_dynamicNodes[p_index].transform = p_transform;
	}
}

void AglScene::update(float p_dt, AglVector3 p_cameraPosition)
{
	for (unsigned int i = 0; i < m_dynamicNodes.size(); i++)
	{
		m_dynamicNodes[i].animated = false;
	}
	if (m_animations.size() > 0)
		m_animations[m_currentAnimation]->update(p_dt);
	for (unsigned int i = 0; i < m_particleSystems.size(); i++)
		m_particleSystems[i]->update(p_dt, p_cameraPosition);
}
AglMaterial* AglScene::getMaterial(int p_index)
{
	return m_materials[p_index];
}
vector<AglMaterial*> AglScene::getMaterials()
{
	return m_materials;
}
vector<AglGradient*> AglScene::getGradients()
{
	return m_gradients;
}
AglGradient* AglScene::getGradient(int p_index)
{
	return m_gradients[p_index];
}
AglParticleSystem* AglScene::getParticleSystem(int p_index)
{
	return m_particleSystems[p_index];
}
string AglScene::getName(int p_index, bool p_removePath)
{
	if (p_removePath)
	{
		string s = m_names[p_index];
		int ind = s.find_last_of('\\');
		if (ind == -1)
			ind = s.find_last_of('/');
		if (ind != -1)
			s = s.substr(ind+1, s.size() - ind);
		return s;
	}
	return m_names[p_index];
}
vector<AglMesh*> AglScene::getMeshes()
{
	return m_meshes;
}
vector<AglSkeletonMapping*> AglScene::getSkeletonMappings()
{
	return m_skeletonMappings;
}
vector<AglMaterialMapping> AglScene::getMaterialMappings()
{
	return m_materialMappings;
}
vector<AglLooseBspTree*> AglScene::getBspTrees()
{
	return m_bspTrees;
}
vector<AglInteriorSphereGrid*> AglScene::getSphereGrids()
{
	return m_sphereGrids;
}

vector<AglConnectionPoint> AglScene::getConnectionPoints()
{
	return m_connectionPoints;
}

AglConnectionPoint AglScene::getConnectionPoint(unsigned int p_index)
{
	return m_connectionPoints[p_index];
}

unsigned int AglScene::getConnectionPointCount()
{
	return m_connectionPoints.size();
}

int AglScene::addName(string p_name)
{
	if (p_name.compare("") == 0)
		p_name = "NoName";
	for (unsigned int i = 0; i < m_names.size(); i++)
	{
		if (m_names[i].compare(p_name) == 0)
			return i;
	}
	m_names.push_back(p_name);
	return m_names.size() - 1;
}

void AglScene::setName(int p_index, string p_name)
{
	m_names[p_index] = p_name;
}
void AglScene::addParticleSystem(AglParticleSystem* pParticleSystem)
{
	m_particleSystems.push_back(pParticleSystem);
}
void AglScene::addMesh(AglMesh* p_mesh)
{
	m_meshes.push_back(p_mesh);
}
void AglScene::addMaterial(AglMaterial* p_material)
{
	p_material->id = m_materials.size();
	m_materials.push_back(p_material);
}
void AglScene::addMaterialMapping(AglMaterialMapping p_materialMapping)
{
	m_materialMappings.push_back(p_materialMapping);
}
void AglScene::addNode(AglNode p_node)
{
	m_nodes.push_back(p_node);
}
void AglScene::addSkeleton(AglSkeleton* p_skeleton)
{
	m_skeletons.push_back(p_skeleton);
}
void AglScene::addSkeletonMapping(AglSkeletonMapping* p_skeletonMapping)
{
	m_skeletonMappings.push_back(p_skeletonMapping);
}
void AglScene::addNodeAnimation(AglNodeAnimation* p_nodeAnimation)
{
	m_nodeAnimations.push_back(p_nodeAnimation);
}
void AglScene::addAnimationLayer(AglAnimationLayer* p_layer)
{
	m_animationLayers.push_back(p_layer);
}
void AglScene::addAnimation(AglAnimation* p_animation)
{
	m_animations.push_back(p_animation);
}
void AglScene::addBspTree(AglLooseBspTree* p_bspTree)
{
	m_bspTrees.push_back(p_bspTree);
}
void AglScene::addSphereGrid(AglInteriorSphereGrid* p_sphereGrid)
{
	m_sphereGrids.push_back(p_sphereGrid);
}
void AglScene::addConnectionPoint(AglConnectionPoint p_connectionPoint)
{
	m_connectionPoints.push_back(p_connectionPoint);
}
int AglScene::addGradient(AglGradient* p_gradient)
{
	m_gradients.push_back(p_gradient);
	return m_gradients.size()-1;
}
AglSceneDesc AglScene::getSceneData()
{
	AglSceneDesc desc;
	desc.animationLayers = this->m_animationLayers;
	desc.animations = this->m_animations;
	desc.materialMappings = this->m_materialMappings;
	desc.materials = this->m_materials;
	desc.meshes = this->m_meshes;
	desc.particleSystems = this->m_particleSystems;
	desc.names = this->m_names;
	desc.nodeAnimations = this->m_nodeAnimations;
	desc.nodes = this->m_nodes;
	desc.skeletonMappings = this->m_skeletonMappings;
	desc.skeletons = this->m_skeletons;
	desc.bspTrees = this->m_bspTrees;
	desc.sphereGrids = this->m_sphereGrids;
	desc.connectionPoints = this->m_connectionPoints;
	desc.particleSystems = this->m_particleSystems;
	desc.coordinateSystem = this->m_coordinateSystem;
	return desc;
}

AglMatrix AglScene::getCoordinateSystemAsMatrix()
{
	AglVector3 right = AglVector3::crossProduct(
						m_coordinateSystem.upVector,
						m_coordinateSystem.forwardVector);
	if (m_coordinateSystem.handedness == AglCoordinateSystem::RIGHT)
		right = -right;

	AglMatrix mat = AglMatrix::identityMatrix();
	mat.SetRight(right);
	mat.SetUp(m_coordinateSystem.upVector);
	mat.SetForward(m_coordinateSystem.forwardVector);
	return mat;
}

void AglScene::setCoordinateSystem( AglCoordinateSystem p_system )
{
	m_coordinateSystem = p_system;
}

bool AglScene::isLeftHanded()
{
	return m_coordinateSystem.handedness == AglCoordinateSystem::LEFT;
}

bool AglScene::isRightHanded()
{
	return m_coordinateSystem.handedness == AglCoordinateSystem::RIGHT;
}
vector<AglParticleSystem*> AglScene::getParticleSystems()
{
	return m_particleSystems;
}
void AglScene::transform(AglMatrix p_transform)
{
	for (unsigned int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i]->transform(p_transform);
	}
	for (unsigned int i = 0; i < m_nodes.size(); i++)
	{
		m_nodes[i].localTransform = p_transform * m_nodes[i].localTransform * p_transform.transpose();
		m_nodes[i].inverseBindMatrix =  p_transform.transpose() * m_nodes[i].inverseBindMatrix * p_transform.transpose();
	}
	for (unsigned int i = 0; i < m_nodeAnimations.size(); i++)
	{
		AglKeyFrame* frames = m_nodeAnimations[i]->getKeyFrames();
		for (unsigned int j = 0; j < m_nodeAnimations[i]->getHeader().keyFrameCount; j++)
		{
			frames[j].transform = p_transform * frames[j].transform * p_transform.transpose();
		}
	}
	for (unsigned int i = 0; i < m_connectionPoints.size(); i++)
	{
		m_connectionPoints[i].transform = p_transform * m_connectionPoints[i].transform * p_transform.transpose();
	}
}
void AglScene::RemoveMaterial(AglMaterial* p_material)
{
	if (m_materials[p_material->id] == p_material)
	{
		//Remove degenerate material mappings
		for (unsigned int i = 0; i < m_materialMappings.size(); i++)
		{
			if (m_materialMappings[i].materialID == p_material->id)
			{
				m_materialMappings[i] = m_materialMappings.back();
				m_materialMappings.pop_back();
				i--;
			}
		}
		//Remove material
		int id = p_material->id;
		m_materials.back()->id = id;
		m_materials[m_materials.back()->id] = m_materials.back();
		delete p_material;
		m_materials.pop_back();

		if (m_materials.size() > 0)
		{
			//Reassign moved material
			for (unsigned int i = 0; i < m_materialMappings.size(); i++)
			{
				if (m_materialMappings[i].materialID == m_materials.size())
				{
					m_materialMappings[i].materialID = id;
				}
			}
		}
		else
		{
			m_materialMappings.clear();
		}
	}
}
void AglScene::RemoveParticleEffect(AglParticleSystem* p_particleSystem)
{
	for (unsigned int i = 0; i < m_particleSystems.size(); i++)
	{
		if (m_particleSystems[i] == p_particleSystem)
		{
			delete m_particleSystems[i];
			m_particleSystems[i] = m_particleSystems.back();
			m_particleSystems.pop_back();
			break;
		}
	}
}
