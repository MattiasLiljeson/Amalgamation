#pragma once
#include <EntitySystem.h>
class EntityFactory;
class GraphicsBackendSystem;
// =======================================================================================
// PreloaderSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Pre-loads necessary assemblages and meshes.
///        
/// # PreloaderSystem
/// Detailed description...
/// Created on: 18-3-2013 
///---------------------------------------------------------------------------------------
class PreloaderSystem: public EntitySystem
{
public:
	PreloaderSystem();
	~PreloaderSystem();
	void initialize() final;
	void initAssemblages();
	void moduleAssemblages();
	void levelAssemblages();
	void unsortedAssemblages();
	void loadModuleMeshes();
	void initMeshes();
private:
	EntityFactory* m_entityFactory;
	GraphicsBackendSystem* m_gfxBackend;
};