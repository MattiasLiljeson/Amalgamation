#include "PhysicsController2.h"
#include "Timer.h"
#include "DX11Application.h"

PhysicsController2::PhysicsController2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	mDevice = pDevice;
	mDeviceContext = pDeviceContext;
	mDebugBox = new DebugBox(pDevice, pDeviceContext);
	mDebugSphere = new DebugSphere(20, pDevice, pDeviceContext);
	mTimeAccum = 0;
	mController = new PhysicsController();
}
PhysicsController2::~PhysicsController2()
{
	Clear();
	delete mDebugBox;
	delete mDebugSphere;
	delete mController;
}

int PhysicsController2::AddSphere(AglVector3 pPosition, float pRadius, bool pUserControlled, CompoundBody* p_parent)
{
	return mController->AddSphere(pPosition, pRadius, pUserControlled, p_parent);
}
int PhysicsController2::AddBox(AglVector3 pPosition, AglVector3 pSize, float pMass, AglVector3 pVelocity, AglVector3 pAngularVelocity, bool pStatic)
{
	return mController->AddBox(pPosition, pSize, pMass, pVelocity, pAngularVelocity, pStatic);
}
int PhysicsController2::AddConvexHull(AglVector3 pPosition, float pSize, float pMass, AglVector3 pVelocity, AglVector3 pAngularVelocity, bool pStatic)
{
	int h = mController->AddConvexHull(pPosition, pSize, pMass, pVelocity, pAngularVelocity, pStatic);
	RigidBodyConvexHull* h2 = (RigidBodyConvexHull*)mController->getBody(h);
	mDebugHullData.push_back(new DebugHullData(h2->getShape(), mDevice, mDeviceContext));
	mDebugHullData.back()->AddInstance(h2);
	return h;
}
int PhysicsController2::AddCompoundBody(AglVector3 p_position)
{
	return mController->AddCompoundBody(p_position);
}

int PhysicsController2::AddMeshBody(AglVector3 pPosition, AglOBB pOBB, AglBoundingSphere pBoundingSphere, AglLooseBspTree* pBSPTree,
								   AglInteriorSphereGrid* pSphereGrid)
{
	return mController->AddMeshBody(pPosition, pOBB, pBoundingSphere, pBSPTree, pSphereGrid);
}

void PhysicsController2::Update(float pElapsedTime)
{
	mController->Update(pElapsedTime);
}
void PhysicsController2::DrawDebug()
{
	vector<RigidBody*> bodies = mController->getBodies();
	for (unsigned int i = 0; i < bodies.size(); i++)
	{
		if (bodies[i]->GetType() == BOX)
		{
			RigidBodyBox* b = (RigidBodyBox*)bodies[i];

			AglMatrix m = b->GetWorld();
			AglMatrix m2 = AglMatrix(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8],
								m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
			
			m = b->GetSizeAsMatrix();
			AglMatrix size = AglMatrix(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8],
								m[9], m[10], m[11], m[12], m[13], m[14], m[15]);

			mDebugBox->draw((AglMatrix)(size * m2));
			if (!bodies[i]->IsStatic())
			{
				float radius = b->GetBoundingSphere().radius;
				m2 = AglMatrix(radius, 0, 0, 0, 0, radius, 0, 0, 0, 0, radius, 0, 0, 0, 0, 1) * m2;
				mDebugSphere->draw(m2, true);
			}
		}
		else if (bodies[i]->GetType() == SPHERE)
		{
			RigidBodySphere* s = (RigidBodySphere*)bodies[i];
			AglMatrix m = s->GetWorld();
			AglMatrix m2 = AglMatrix(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8],
								m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
			mDebugSphere->draw((AglMatrix)(m2 * s->GetRadius()));
		}
		else if (bodies[i]->GetType() == MESH)
		{
			RigidBodyMesh* m = (RigidBodyMesh*)bodies[i];
			AglMatrix world = m->GetWorld();
			AglBoundingSphere bs = m->GetBoundingSphere();
			AglMatrix mat;
			AglMatrix::componentsToMatrix(mat, AglVector3(bs.radius, bs.radius, bs.radius),
											AglQuaternion::identity(), bs.position);
			//mDebugSphere->draw(mat, true);
			AglOBB obb = m->GetOBB();

			AglMatrix::componentsToMatrix(mat, obb.size,
				AglQuaternion::identity(), AglVector3(0, 0, 0));

			mat = mat * obb.world;
			//mat *= world;
			mat.SetTranslation(obb.world.GetTranslation());
			mDebugBox->draw(mat, true);
		}
	}
	for (unsigned int i = 0; i < mDebugHullData.size(); i++)
	{
		mDebugHullData[i]->Draw(mDebugSphere, mDebugBox);
	}
}
void PhysicsController2::Clear()
{
	mController->Clear();
	for (unsigned int i = 0; i < mDebugHullData.size(); i++)
		delete mDebugHullData[i];
	mDebugHullData.clear();
}
float PhysicsController2::RaysVsObjects(vector<PhyRay> rays, RigidBody* p_ignore, AglBoundingSphere p_sphere)
{
	return mController->RaysVsObjects(rays, p_ignore, p_sphere);
}

void PhysicsController2::DetachBodyFromCompound(CompoundBody* p_compound, RigidBody* p_body)
{
	mController->DetachBodyFromCompound(p_compound, p_body);
}