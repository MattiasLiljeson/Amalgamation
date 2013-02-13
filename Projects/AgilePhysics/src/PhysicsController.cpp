#include "PhysicsController.h"

PhysicsController::PhysicsController(): COLLISION_REPETITIONS(5)
{
	mTimeAccum = 0;
	mStaticBodies = NULL;
	mStaticBodies = new AxisPruner();
}
PhysicsController::~PhysicsController()
{
	Clear();
}

int PhysicsController::AddSphere(AglVector3 pPosition, float pRadius, bool pUserControlled, CompoundBody* pParent)
{
	RigidBodySphere* s = new RigidBodySphere(pPosition, pRadius, pUserControlled);
	mRigidBodies.push_back(pair<RigidBody*, unsigned int>(s, mBodies.size()));
	mBodies.push_back(s);
	if (pParent)
		pParent->AddChild(s);
	return mBodies.size()-1;
}

int PhysicsController::AddSphere(AglMatrix p_world, float p_radius, float p_mass, AglVector3 p_velocity, AglVector3 p_angularVelocity, bool p_static, CompoundBody* pParent, bool pImpulseEnabled,
			  bool pCollisionEnabled)
{
	RigidBodySphere* s = new RigidBodySphere(p_world, p_radius, p_mass, p_velocity, p_angularVelocity, p_static, pParent, pImpulseEnabled);
	s->SetCollisionEnabled(pCollisionEnabled);
	if (!p_static)
		mRigidBodies.push_back(pair<RigidBody*, unsigned int>(s, mBodies.size()));
	else
	{
		mStaticBodies->Insert(s);
	}
	mBodies.push_back(s);
	if (pParent)
		pParent->AddChild(s);
	return mBodies.size()-1;
}

int PhysicsController::AddBox(AglVector3 pPosition, AglVector3 pSize, float pMass, AglVector3 pVelocity, 
								AglVector3 pAngularVelocity, bool pStatic, CompoundBody* pParent, bool pImpulseEnabled, bool pCollisionEnabled)
{
	RigidBodyBox* b = new RigidBodyBox(pPosition, pSize, pMass, pVelocity, pAngularVelocity, pStatic, pImpulseEnabled);
	b->SetCollisionEnabled(pCollisionEnabled);
	if (!pStatic)
		mRigidBodies.push_back(pair<RigidBody*, unsigned int>(b, mBodies.size()));
	else
	{
		mStaticBodies->Insert(b);
	}
	mBodies.push_back(b);
	if (pParent)
		pParent->AddChild(b);
	return mBodies.size()-1;
}
int PhysicsController::AddBox(AglOBB p_shape, float p_mass, AglVector3 p_velocity, AglVector3 p_angularVelocity, bool p_static, 
							  CompoundBody* pParent, bool pImpulseEnabled, bool pCollisionEnabled)
{
	RigidBodyBox* b = new RigidBodyBox(p_shape, p_mass, p_velocity, p_angularVelocity, p_static, pImpulseEnabled);
	b->SetCollisionEnabled(pCollisionEnabled);
	if (!p_static)
		mRigidBodies.push_back(pair<RigidBody*, unsigned int>(b, mBodies.size()));
	else
	{
		mStaticBodies->Insert(b);
	}
	mBodies.push_back(b);
	if (pParent)
		pParent->AddChild(b);
	return mBodies.size()-1;
}

int PhysicsController::AddBox(AglMatrix p_world, AglVector3 p_size, float p_mass, AglVector3 p_velocity, AglVector3 p_angularVelocity, bool p_static, CompoundBody* pParent, bool pImpulseEnabled,
		   bool pCollisionEnabled)
{
	BoxInitData init;
	init.World = p_world;
	init.Mass = p_mass;
	init.Velocity = p_velocity;
	init.AngularVelocity = p_angularVelocity;
	init.Static = p_static;
	init.ImpulseEnabled = pImpulseEnabled;
	init.Parent = pParent;
	init.CollisionEnabled = pCollisionEnabled;
	init.Size = p_size;
	RigidBodyBox* b = new RigidBodyBox(init);
	b->SetCollisionEnabled(pCollisionEnabled);
	if (!p_static)
		mRigidBodies.push_back(pair<RigidBody*, unsigned int>(b, mBodies.size()));
	else
	{
		mStaticBodies->Insert(b);
	}
	mBodies.push_back(b);
	if (pParent)
		pParent->AddChild(b);
	return mBodies.size()-1;
}
int PhysicsController::AddConvexHull(AglVector3 pPosition, float pSize, float pMass, AglVector3 pVelocity, AglVector3 pAngularVelocity, bool pStatic, CompoundBody* pParent)
{
	//Create a new convex hull shape.
	ConvexHullShape* shape = new ConvexHullShape(10);
	mConvexHullShapes.push_back(shape);

	RigidBodyConvexHull* h = new RigidBodyConvexHull(shape, pPosition, pSize, pMass, pVelocity, pAngularVelocity, pStatic);
	mRigidBodies.push_back(pair<RigidBody*, unsigned int>(h, mBodies.size()));
	mBodies.push_back(h);
	if (pParent)
		pParent->AddChild(h);
	return mBodies.size()-1;
}
int PhysicsController::AddCompoundBody(AglMatrix p_transform)
{
	CompoundBody* cb = new CompoundBody(p_transform);
	mBodies.push_back(cb);
	mCompoundBodies.push_back(cb);
	return mBodies.size()-1;
}
int PhysicsController::AddMeshBody(AglVector3 pPosition, AglOBB pOBB, AglBoundingSphere pBoundingSphere, AglLooseBspTree* pBSPTree,
	AglInteriorSphereGrid* pSphereGrid)
{
	RigidBodyMesh* rbm = new RigidBodyMesh(pPosition, pOBB, pBoundingSphere, pBSPTree, pSphereGrid);
	mRigidBodies.push_back(pair<RigidBody*, unsigned int>(rbm, mBodies.size()));
	mBodies.push_back(rbm);
	return mBodies.size()-1;
}
int PhysicsController::AddMeshBody(AglMatrix pWorld, AglOBB pOBB, AglBoundingSphere pBoundingSphere, AglVector3 pSize, AglLooseBspTree* pBSPTree)
{
	RigidBodyMesh* m = new RigidBodyMesh(pWorld, pOBB, pBoundingSphere, pSize, pBSPTree);
	m->SetCollisionEnabled(true);
	m->Activate();
	//Meshes are always static
	mStaticBodies->Insert(m);
	mBodies.push_back(m);
	return mBodies.size()-1;
}

int PhysicsController::AddLineSegment(AglVector3 p_p1, AglVector3 p_p2)
{
	LineSegment ls;
	ls.p1 = p_p1;
	ls.p2 = p_p2;
	mLineSegments.push_back(ls);
	return mLineSegments.size()-1;
}
void PhysicsController::SetLineSegment(AglVector3 p_p1, AglVector3 p_p2, int p_index)
{
	mLineSegments[p_index].p1 = p_p1;
	mLineSegments[p_index].p2 = p_p2;
}
int PhysicsController::AddRay(AglVector3 p_o, AglVector3 p_dir, float maxLength)
{
	LineSegment ls;
	ls.p1 = p_o;
	ls.p2 = p_o + p_dir * maxLength;
	mLineSegments.push_back(ls);
	return mLineSegments.size()-1;
}
void PhysicsController::SetRay(AglVector3 p_o, AglVector3 p_dir, int p_index, float maxLength)
{
	mLineSegments[p_index].p1 = p_o;
	mLineSegments[p_index].p2 = p_o + p_dir * maxLength;
}
void PhysicsController::GetRay(unsigned int p_index, AglVector3& p_o, AglVector3& p_d)
{
	p_o = mLineSegments[p_index].p1;
	p_d = mLineSegments[p_index].p2 - mLineSegments[p_index].p1;
	p_d.normalize();
}

void PhysicsController::DetachBodyFromCompound(RigidBody* p_body, bool p_impulseEnabled, CompoundBody* p_compound)
{
	if (p_compound)
		p_compound->DetachChild(p_body);
	else
	{
		CompoundBody* parent = p_body->GetParent();
		parent->DetachChild(p_body);
	}
	p_body->SetImpulseEnabled(p_impulseEnabled);
}

void PhysicsController::Update(float pElapsedTime)
{
	mTimeAccum += pElapsedTime;
	if (mTimeAccum < 0.005f)
		return;
	pElapsedTime = mTimeAccum;
	while (mTimeAccum > 0.005f)
		mTimeAccum -= 0.005f;


	mCollisions.clear();
	mLineSegmentCollisions.clear();
	//New update method stepping
	//1) Update Velocity and Position
	//2) Perform collision detection
	//3) Revert positions and velocities
	//4) Solve collisions
	//5) Update velocities
	//6) Solve contacts
	//7) Perform shock step (Ignored for now)
	//8) Update positions
	//9) Scramble. Resort the bodies to create a "fair" collision stepping

	//1) Update Velocities and Positions
	for (unsigned int i = 0; i < mBodies.size(); i++)
	{
		if (!mBodies[i]->IsStatic())
		{
			mBodies[i]->UpdateVelocity(pElapsedTime);
			mBodies[i]->UpdatePosition(pElapsedTime);
		}
	}

	//2) Check for collisions
	vector<PhyCollisionData> collisions;
	for (unsigned int i = 0; i < mRigidBodies.size(); i++)
	{
		if (mRigidBodies[i].first->IsActive() && mRigidBodies[i].first->IsCollisionEnabled())
		{
			//Check collisions against other rigid bodies
			for (unsigned int j = i + 1; j < mRigidBodies.size(); j++)
			{
				if (mRigidBodies[j].first->IsActive() && mRigidBodies[j].first->IsCollisionEnabled())
				{
					if (!mRigidBodies[i].first->IsStatic() || !mRigidBodies[j].first->IsStatic())
					{
						if (mRigidBodies[i].first->GetParent() == mRigidBodies[j].first->GetParent() && 
							mRigidBodies[i].first->GetParent() != NULL)
							continue;
						PhyCollisionData colData;
						if (CheckCollision(mRigidBodies[i].first, mRigidBodies[j].first, &colData))
						{
							theGlobal = 1;
							//Reg collision
							mCollisions.push_back(UintPair(mRigidBodies[i].second, mRigidBodies[j].second));

							if (mRigidBodies[i].first->IsImpulseEnabled() && mRigidBodies[j].first->IsImpulseEnabled())
							{
								//Solves high impulse issues for boxes
								if (colData.Contacts.size() == 4)
								{
									pair<AglVector3, AglVector3> av;
									av.first = AglVector3(0, 0, 0);
									av.second = AglVector3(0, 0, 0);
									for (int i = 0; i < 4; i++)
									{
										av.first += colData.Contacts[i].first;
										av.second += colData.Contacts[i].second;
									}
									av.first *= 0.25f;
									av.second *= 0.25f;
									pair<AglVector3, AglVector3> temp = colData.Contacts[0];
									colData.Contacts[0] = av;
									colData.Contacts.push_back(temp);		
								}
								collisions.push_back(colData);
							}
						}
					}
				}
			}
			//Check for collision against line segments
			for (unsigned int j = 0; j < mLineSegments.size(); j++)
			{
				float t;
				if (CheckCollision(mLineSegments[j], mRigidBodies[i].first, t))
				{
					LineCollisionData lcd;
					lcd.bodyID = mRigidBodies[i].second;
					lcd.lineID = j;
					lcd.t = t;
					mLineSegmentCollisions.push_back(lcd);
				}
			}
			//Check for collision against static geometry
		/*	if (mStaticBodies)
			{
				vector<PhyCollisionData> staticCol = mStaticBodies->Query(mRigidBodies[i].first);
				for (unsigned int j = 0; j < staticCol.size(); j++)
				{
					collisions.push_back(staticCol[j]);
				}
			} */
		}
	}

	//3) Revert positions and velocities
	for (unsigned int i = 0; i < mBodies.size(); i++)
	{
		if (!mBodies[i]->IsStatic())
		{
			mBodies[i]->RevertVelocity();
			mBodies[i]->RevertPosition();
		}
	}
	
	//4) Solve collisions
	unsigned int rep = COLLISION_REPETITIONS;
	while (rep > 0)
	{
		for (unsigned int i = 0; i < collisions.size(); i++)
		{
			ResolveCollision(&collisions[i], pElapsedTime, 0.1f);
		}
		rep--;
	}
	
	//5) Update Velocities
	for (unsigned int i = 0; i < mBodies.size(); i++)
	{
		if (!mBodies[i]->IsStatic())
			mBodies[i]->UpdateVelocity(pElapsedTime);
	}

	//6) Solve contacts
	rep = COLLISION_REPETITIONS;
	while (rep > 0)
	{
		for (unsigned int i = 0; i < collisions.size(); i++)
		{
			ResolveCollision(&collisions[i], pElapsedTime, 0.0f);
		}
		rep--;
	}


	//7) Perform shock step
	bool shockstep = false;
	if (shockstep)
	{

		//Sort the collisions
		for (unsigned int i = 0; i < collisions.size(); i++)
		{
			for (unsigned int j = 0; j < collisions.size()-1; j++)
			{
				float min1 = min(collisions[j].Body1->GetPosition().y, 
									collisions[j].Body2->GetPosition().y);
				float min2 = min(collisions[j+1].Body1->GetPosition().y, 
					collisions[j+1].Body2->GetPosition().y);
				if (min2 < min1)
				{
					PhyCollisionData t = collisions[j];
					collisions[j] = collisions[j+1];
					collisions[j+1] = t;

				}

			}
		}
		//Perform shock step - Not fully though out! Seems to fix a lot though
		//Should look into alternative resolve methods.
		//Angular velocity should not be affected. Turns out that not affecting
		//angular velocity gave weird results
		for (unsigned int i = 0; i < collisions.size(); i++)
		{
			RigidBody* b1 = collisions[i].Body1;
			float k1 = AglVector3::dotProduct(b1->GetVelocity(), b1->GetVelocity()) + AglVector3::dotProduct(b1->GetAngularVelocity(), b1->GetAngularVelocity());

			RigidBody* b2 = collisions[i].Body2;
			float k2 = AglVector3::dotProduct(b2->GetVelocity(), b2->GetVelocity()) + AglVector3::dotProduct(b2->GetAngularVelocity(), b2->GetAngularVelocity());

			if (k1 + k2 < 0.5f * ONE_METER)
			{
				ResolveCollision(&collisions[i], pElapsedTime, 0.0f);
				collisions[i].Body2->SetTempStatic(true);
				collisions[i].Body1->SetTempStatic(true);
			}
		}
	}


	//8) Update positions
	for (unsigned int i = 0; i < mBodies.size(); i++)
	{
		if (typeid(*mBodies[i]) == typeid(RigidBody))
			((RigidBody*)mBodies[i])->SetTempStatic(false);

		if (!mBodies[i]->IsStatic())
			mBodies[i]->UpdatePosition(pElapsedTime);
	}

	//9) Scramble the bodies
	for (unsigned int i = 0; i < mRigidBodies.size(); i++)
	{
		int rnd1 = rand() % mRigidBodies.size();
		int rnd2 = rand() % mRigidBodies.size();
		pair<RigidBody*, unsigned int> t = mRigidBodies[rnd1];
		mRigidBodies[rnd1] = mRigidBodies[rnd2];
		mRigidBodies[rnd2] = t;
	}
}
void PhysicsController::Clear()
{
	for (unsigned int i = 0; i < mBodies.size(); i++)
	{
		if (mBodies[i]->IsCompoundBody())
		{
			CompoundBody* cp = (CompoundBody*)mBodies[i];
			delete cp;
		}
		else
		{
			RigidBody* rb = (RigidBody*)mBodies[i];
			delete rb;
		}
	}
	for (unsigned int i = 0; i < mConvexHullShapes.size(); i++)
		delete mConvexHullShapes[i];

	mRigidBodies.clear();
	mCompoundBodies.clear();
	mConvexHullShapes.clear();
	delete mStaticBodies;
	mStaticBodies = NULL;
}
float PhysicsController::RaysVsObjects(vector<PhyRay> rays, RigidBody* p_ignore, AglBoundingSphere p_sphere)
{
	vector<RigidBody*> toCheck;
	AglVector3 nothing;
	for (unsigned int i = 0; i < mRigidBodies.size(); i++)
	{
		if (CheckCollision(p_sphere, mRigidBodies[i].first, nothing))
		{
			toCheck.push_back(mRigidBodies[i].first);
		}
	}
	float minT = 1;
	for (unsigned int i = 0; i < rays.size(); i++)
	{
		for (unsigned int j = 0; j < toCheck.size(); j++)
		{

			RayCollisionData data;
			if (toCheck[j] != p_ignore && CheckCollision(rays[i], toCheck[j], &data))
			{
				minT = min(minT, data.t);
			}
		}
	}
	return minT;
}

int PhysicsController::FindClosestCollision(AglVector3 p_p1, AglVector3 p_p2, int p_avoid)
{
	LineSegment ls;
	ls.p1 = p_p1;
	ls.p2 = p_p2;

	int col = -1;
	float closestT = FLT_MAX;
	for (unsigned int i = 0; i < mRigidBodies.size(); i++)
	{
		float t;
		if (mRigidBodies[i].second != p_avoid && CheckCollision(ls, mRigidBodies[i].first, t))
		{
			if (t < closestT)
			{
				col = mRigidBodies[i].second;
				closestT = t;
			}
		}
	}
	return col;
}

void PhysicsController::ApplyExternalImpulse(int p_id, AglVector3 p_impulse, AglVector3 p_angularImpulse)
{
	mBodies[p_id]->AddImpulse(p_impulse);
	mBodies[p_id]->AddAngularImpulse(p_angularImpulse);
}
void PhysicsController::ApplyExternalImpulse(AglVector3 p_position, float p_radius, float p_magnitude)
{
	AglBoundingSphere bs(p_position, p_radius);
	for (unsigned int i = 0; i < mRigidBodies.size(); i++)
	{
		AglVector3 colPoint;
		if (CheckCollision(bs, mRigidBodies[i].first, colPoint))
		{
			AglVector3 n = colPoint - bs.position;
			float dist = n.length();
			n.normalize();

			AglVector3 rA = colPoint - mRigidBodies[i].first->GetCenterOfMass();

			//Add mass as a resistance factor
			float kn = mRigidBodies[i].first->GetInvMass();

			//Add inertia tensors as resistance factors 
			float knAng = AglVector3::dotProduct(n, 
				AglVector3::crossProduct(AglVec3Transform(AglVector3::crossProduct(rA, n), mRigidBodies[i].first->GetInvInertiaWorld()), rA));

			kn += knAng;

			//Power
			float dVn = p_magnitude * p_radius / dist;

			//Calculate final normal impulse
			float pn = dVn / kn;

			//Apply impulses in normal direction
			mRigidBodies[i].first->AddImpulse(n * pn);
			AglVector3 angImpulse = AglVec3TransformNormal(AglVector3::crossProduct(rA, n * pn), mRigidBodies[i].first->GetInvInertiaWorld());
			mRigidBodies[i].first->AddAngularImpulse(angImpulse);



			/*AglVector3 dir = mRigidBodies[i].first->GetCenterOfMass() - p_position;
			float l = dir.length();
			dir.normalize();
			AglVector3 dir2 = colPoint - bs.position;

			AglVector3 rotAxis = AglVector3::crossProduct(dir2, dir);
			rotAxis.normalize();

			mRigidBodies[i].first->AddImpulse(dir*p_magnitude/l);
			mRigidBodies[i].first->AddAngularImpulse(rotAxis*p_magnitude/l);*/
		}
	}
}

bool PhysicsController::IsColliding(unsigned int p_b1, unsigned int p_b2)
{
	if (p_b1 > p_b2)
	{
		unsigned int temp = p_b2;
		p_b2 = p_b1;
		p_b1 = temp;
	}
	for (unsigned int i = 0; i < mCollisions.size(); i++)
	{
		if (mCollisions[i].first == p_b1)
			if (mCollisions[i].second == p_b2)
				return true;
	}
	return false;
}

vector<unsigned int> PhysicsController::CollidesWith(unsigned int p_b)
{
	if (mBodies[p_b]->IsCompoundBody())
	{
		CompoundBody* body = (CompoundBody*)mBodies[p_b];
		vector<RigidBody*> children = body->GetChildren();
		vector<unsigned int> list;
		for (unsigned int i = 0; i < children.size(); i++)
		{
			for (unsigned int j = 0; j < mCollisions.size(); j++)
			{
				RigidBody* b = (RigidBody*)mBodies[mCollisions[j].first];
				if (b == children[i])
				{
					list.push_back(mCollisions[j].second);
				}
				b = (RigidBody*)mBodies[mCollisions[j].second];
				if (b == children[i])
				{
					list.push_back(mCollisions[j].first);
				}
			}
		}
		return list;
	}
	else
	{
		vector<unsigned int> list;
		for (unsigned int i = 0; i < mCollisions.size(); i++)
		{
			if (mCollisions[i].first == p_b)
				list.push_back(mCollisions[i].second);
			if (mCollisions[i].second == p_b)
				list.push_back(mCollisions[i].first);
		}
		return list;
	}
}
vector<unsigned int> PhysicsController::LineCollidesWith(unsigned int p_line)
{
	vector<unsigned int> cols;
	for (unsigned int i = 0; i < mLineSegmentCollisions.size(); i++)
	{
		if (mLineSegmentCollisions[i].lineID == p_line)
			cols.push_back(mLineSegmentCollisions[i].bodyID);
	}
	return cols;
}
int	PhysicsController::LineClosestCollision(unsigned int p_line)
{
	int col = -1;
	float closestT = FLT_MAX;
	for (unsigned int i = 0; i < mLineSegmentCollisions.size(); i++)
	{
		if (mLineSegmentCollisions[i].lineID == p_line)
		{
			if (mLineSegmentCollisions[i].t < closestT)
			{
				col = mLineSegmentCollisions[i].bodyID;
				closestT = mLineSegmentCollisions[i].t;
			}
		}
	}
	return col;
}
vector<LineCollisionData> PhysicsController::LineSortedCollisions(unsigned int p_line) //Sorted with the closest first
{
	vector<LineCollisionData> cols;
	for (unsigned int i = 0; i < mLineSegmentCollisions.size(); i++)
	{
		if (mLineSegmentCollisions[i].lineID == p_line)
		{
			int curr = cols.size();
			cols.push_back(mLineSegmentCollisions[i]);
			while (curr > 0 && cols[curr].t > cols[curr-1].t)
			{
				LineCollisionData temp = cols[curr];
				cols[curr] = cols[curr-1];
				cols[curr-1] = temp;
			}
		}
	}
	return cols;
}

void PhysicsController::ActivateBody(unsigned int pBody)
{
	mBodies[pBody]->Activate();
}
void PhysicsController::InactivateBody(unsigned int pBody)
{
	mBodies[pBody]->Inactivate();
}
bool PhysicsController::IsActive(unsigned int pBody)
{
	return mBodies[pBody]->IsActive();
}

void PhysicsController::AttachBodyToCompound(CompoundBody* p_compound, RigidBody* p_body, AglMatrix p_localTransform)
{
	p_body->SetImpulseEnabled(true);
	p_compound->AddChild(p_body, p_localTransform);

	unsigned int index = -1;
	//Invalidate all collisions with the body
	for (unsigned int i = 0; i < mBodies.size(); i++)
	{
		if ((RigidBody*)mBodies[i] == p_body)
		{
			index = i;
			break;
		}
	}

	for (unsigned int i = 0; i < mCollisions.size(); i++)
	{
		if (mCollisions[i].first == index ||
			mCollisions[i].second == index)
		{
			mCollisions[i] = mCollisions.back();
			mCollisions.pop_back();
			i--;
		}
	}
}
void PhysicsController::InitStaticBodiesOctree(AglVector3 pMin, AglVector3 pMax)
{
	//if (!mStaticBodies)
		//mStaticBodies = new Octree(4, pMin, pMax);
}